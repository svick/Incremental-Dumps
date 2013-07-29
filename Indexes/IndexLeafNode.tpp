#include <utility>
#include <vector>
#include "../DumpObjects/DumpTraits.h"
#include "../DumpObjects/DumpObjectKind.h"
#include "Iterators/IndexLeafIterator.h"

using std::pair;
using std::vector;

template<typename TKey, typename TValue>
TValue IndexLeafNode<TKey, TValue>::Get(TKey key)
{
    auto found = indexMap.find(key);

    if (found == indexMap.end())
        return TValue();

    return found->second;
}

template<typename TKey, typename TValue>
void IndexLeafNode<TKey, TValue>::Add(TKey key, TValue value)
{
    if (indexMap.size() >= std::numeric_limits<uint16_t>::max())
    {
        throw new DumpException();
    }

    indexMap.insert(pair<TKey, TValue>(key, value));
}

template<typename TKey, typename TValue>
void IndexLeafNode<TKey, TValue>::AddOrUpdate(TKey key, TValue value)
{
    auto pos = indexMap.find(key);
    if (pos == indexMap.end())
    {
        Add(key, value);
    }
    else
    {
        pos->second = value; // will this work?
    }
}

template<typename TKey, typename TValue>
void IndexLeafNode<TKey, TValue>::Remove(const TKey key)
{
    indexMap.erase(key);
}

template<typename TKey, typename TValue>
IndexLeafNode<TKey, TValue>::IndexLeafNode(weak_ptr<WritableDump> dump)
    : IndexNode<TKey, TValue>(dump)
{}

template<typename TKey, typename TValue>
unique_ptr<IndexNode<TKey, TValue>> IndexLeafNode<TKey, TValue>::Read(weak_ptr<WritableDump> dump, istream &stream)
{
    auto node = new IndexLeafNode<TKey, TValue>(dump);

    // DumpObjectKind is assumed to be already read by IndexNode::Read

    node->indexMap = DumpTraits<std::map<TKey, TValue>>::Read(stream);

    return unique_ptr<IndexNode<TKey, TValue>>(node);
}

template<typename TKey, typename TValue>
void IndexLeafNode<TKey, TValue>::WriteInternal()
{
    WriteValue((uint8_t)DumpObjectKind::IndexLeafNode);
    WriteValue(indexMap);
}

template<typename TKey, typename TValue>
uint32_t IndexLeafNode<TKey, TValue>::NewLength()
{
    return this->Size;
}

template<typename TKey, typename TValue>
std::uint32_t IndexLeafNode<TKey, TValue>:: RealLength()
{
    return ValueSize((uint8_t)DumpObjectKind::IndexLeafNode)
        + ValueSize(indexMap);
}

template<typename TKey, typename TValue>
typename IndexNode<TKey, TValue>::SplitResult IndexLeafNode<TKey, TValue>::Split()
{
    auto left = new IndexLeafNode<TKey, TValue>(this->dump);
    auto right = new IndexLeafNode<TKey, TValue>(this->dump);

    auto it = indexMap.begin();
    for (unsigned i = 0; i < (indexMap.size() - 1) / 2; i++)
    {
        left->indexMap.insert(*it);
        it++;
    }

    auto middleKey = it->first;
    left->indexMap.insert(*it);
    it++;

    for (; it != indexMap.end(); it++)
    {
        right->indexMap.insert(*it);
    }

    return SplitResult(
        std::unique_ptr<IndexNode<TKey, TValue>>(left),
        std::unique_ptr<IndexNode<TKey, TValue>>(right),
        middleKey);
}

template<typename TKey, typename TValue>
std::unique_ptr<IndexNodeIterator<TKey, TValue>> IndexLeafNode<TKey, TValue>::begin()
{
    return std::unique_ptr<IndexNodeIterator<TKey, TValue>>(new IndexLeafIterator<TKey, TValue>(indexMap.begin()));
}

template<typename TKey, typename TValue>
std::unique_ptr<IndexNodeIterator<TKey, TValue>> IndexLeafNode<TKey, TValue>::end()
{
    return std::unique_ptr<IndexNodeIterator<TKey, TValue>>(new IndexLeafIterator<TKey, TValue>(indexMap.end()));
}
