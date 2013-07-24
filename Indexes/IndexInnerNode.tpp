#include <algorithm>
#include <utility>
#include <vector>
#include "../DumpObjects/DumpTraits.h"
#include "../DumpObjects/DumpObjectKind.h"
#include "../CollectionHelpers.h"
#include "Iterators/IndexInnerIterator.h"

template<typename TKey, typename TValue>
IndexNode<TKey, TValue>* IndexInnerNode<TKey, TValue>::GetChildByIndex(std::uint16_t index)
{
    if (cachedChildren[index] == nullptr)
    {
        cachedChildren[index] = IndexNode<TKey, TValue>::Read(this->dump, childOffsets[index].value);
    }

    return cachedChildren[index].get();
}

template<typename TKey, typename TValue>
std::uint16_t IndexInnerNode<TKey, TValue>::GetKeyIndex(TKey key)
{
    auto foundRef = std::lower_bound(keys.begin(), keys.end(), key);
    auto foundPos = foundRef - keys.begin();

    return foundPos;
}

template<typename TKey, typename TValue>
TValue IndexInnerNode<TKey, TValue>::Get(TKey key)
{
    auto index = GetKeyIndex(key);
    return GetChildByIndex(index)->Get(key);
}

template<typename TKey, typename TValue>
void IndexInnerNode<TKey, TValue>::AfterAdd(std::uint16_t updatedChildIndex)
{
    auto updatedChild = GetChildByIndex(updatedChildIndex);

    if (updatedChild->IsOversized())
    {
        this->dump.lock()->spaceManager->Delete(updatedChild->SavedOffset(), updatedChild->NewLength());

        auto splitted = updatedChild->Split();
        splitted.LeftNode->Write();
        splitted.RightNode->Write();

        insert_at(keys, updatedChildIndex, splitted.SplitKey);

        childOffsets[updatedChildIndex] = splitted.LeftNode->SavedOffset();
        cachedChildren[updatedChildIndex] = std::move(splitted.LeftNode);

        insert_at(childOffsets, updatedChildIndex + 1, splitted.RightNode->SavedOffset());
        insert_at(cachedChildren, updatedChildIndex + 1, std::move(splitted.RightNode));
    }
}

template<typename TKey, typename TValue>
void IndexInnerNode<TKey, TValue>::Add(TKey key, TValue value)
{
    auto index = GetKeyIndex(key);
    auto child = GetChildByIndex(index);

    child->Add(key, value);

    AfterAdd(index);
}

template<typename TKey, typename TValue>
void IndexInnerNode<TKey, TValue>::AddOrUpdate(TKey key, TValue value)
{
    auto index = GetKeyIndex(key);
    auto child = GetChildByIndex(index);

    child->AddOrUpdate(key, value);

    AfterAdd(index);
}

template<typename TKey, typename TValue>
void IndexInnerNode<TKey, TValue>::Remove(const TKey key)
{
    auto index = GetKeyIndex(key);
    return GetChildByIndex(index)->Remove(key);
}

template<typename TKey, typename TValue>
IndexInnerNode<TKey, TValue>::IndexInnerNode(weak_ptr<WritableDump> dump)
    : IndexNode<TKey, TValue>(dump)
{}

template<typename TKey, typename TValue>
IndexInnerNode<TKey, TValue>::IndexInnerNode(weak_ptr<WritableDump> dump, SplitResult splitResult)
    : IndexNode<TKey, TValue>(dump)
{
    keys.push_back(splitResult.SplitKey);

    auto leftOffset = splitResult.LeftNode->SavedOffset();
    if (leftOffset == 0)
        throw DumpException();
    childOffsets.push_back(leftOffset);
    cachedChildren.push_back(std::move(splitResult.LeftNode));

    auto rightOffset = splitResult.RightNode->SavedOffset();
    if (rightOffset == 0)
        throw DumpException();
    childOffsets.push_back(rightOffset);
    cachedChildren.push_back(std::move(splitResult.RightNode));
}

template<typename TKey, typename TValue>
unique_ptr<IndexNode<TKey, TValue>> IndexInnerNode<TKey, TValue>::Read(weak_ptr<WritableDump> dump, istream &stream)
{
    auto node = new IndexInnerNode<TKey, TValue>(dump);

    // DumpObjectKind is assumed to be already read by IndexNode::Read

    uint16_t count = DumpTraits<uint16_t>::Read(stream);

    for (int i = 0; i < count; i++)
    {
        node->keys.push_back(DumpTraits<TKey>::Read(stream));
    }

    for (int i = 0; i < count + 1; i++)
    {
        node->childOffsets.push_back(DumpTraits<Offset>::Read(stream));
        node->cachedChildren.push_back(std::unique_ptr<IndexNode<TKey, TValue>>());
    }

    return unique_ptr<IndexNode<TKey, TValue>>(node);
}

template<typename TKey, typename TValue>
void IndexInnerNode<TKey, TValue>::WriteInternal()
{
    WriteValue((uint8_t)DumpObjectKind::IndexInnerNode);
    WriteValue((uint16_t)keys.size());

	for (auto key : keys)
    {
		WriteValue(key);
    }

	for (auto offset : childOffsets)
    {
		WriteValue(offset);
    }
}

template<typename TKey, typename TValue>
void IndexInnerNode<TKey, TValue>::Write()
{
    IndexNode<TKey, TValue>::Write();

    for (auto &cachedChild : cachedChildren)
    {
        if (cachedChild != nullptr)
            cachedChild->Write();
    }
}

template<typename TKey, typename TValue>
uint32_t IndexInnerNode<TKey, TValue>::NewLength()
{
    return this->Size;
}

template<typename TKey, typename TValue>
std::uint32_t IndexInnerNode<TKey, TValue>::RealLength()
{
    return DumpTraits<uint8_t>::DumpSize((uint8_t)DumpObjectKind::IndexLeafNode)
        + DumpTraits<uint16_t>::DumpSize(keys.size())
        + keys.size() * DumpTraits<TKey>::DumpSize()
        + (keys.size() + 1) * DumpTraits<Offset>::DumpSize();
}

template<typename TKey, typename TValue>
typename IndexNode<TKey, TValue>::SplitResult IndexInnerNode<TKey, TValue>::Split()
{
    auto left = new IndexInnerNode<TKey, TValue>(this->dump);
    auto right = new IndexInnerNode<TKey, TValue>(this->dump);

    auto middle = keys.size() / 2;

    unsigned i = 0;

    for (; i < middle; i++)
    {
        left->keys.push_back(keys[i]);
        left->childOffsets.push_back(childOffsets[i]);
        left->cachedChildren.push_back(std::move(cachedChildren[i]));
    }

    left->childOffsets.push_back(childOffsets[i]);
    left->cachedChildren.push_back(std::move(cachedChildren[i]));

    TKey middleKey = keys[i];

    i++;

    for (; i < keys.size(); i++)
    {
        right->keys.push_back(keys[i]);
        right->childOffsets.push_back(childOffsets[i]);
        right->cachedChildren.push_back(std::move(cachedChildren[i]));
    }

    right->childOffsets.push_back(childOffsets[i]);
    right->cachedChildren.push_back(std::move(cachedChildren[i]));

    return SplitResult(
        std::unique_ptr<IndexNode<TKey, TValue>>(left),
        std::unique_ptr<IndexNode<TKey, TValue>>(right),
        middleKey);
}

template<typename TKey, typename TValue>
shared_ptr<IndexNodeIterator<TKey, TValue>> IndexInnerNode<TKey, TValue>::begin()
{
    return shared_ptr<IndexNodeIterator<TKey, TValue>>(new IndexInnerIterator<TKey, TValue>(this, true));
}

template<typename TKey, typename TValue>
shared_ptr<IndexNodeIterator<TKey, TValue>> IndexInnerNode<TKey, TValue>::end()
{
    return shared_ptr<IndexNodeIterator<TKey, TValue>>(new IndexInnerIterator<TKey, TValue>(this, false));
}
