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
    if (indexMap.size() >= Size)
    {
        // TODO
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
{
}

template<typename TKey, typename TValue>
unique_ptr<IndexNode<TKey, TValue>> IndexLeafNode<TKey, TValue>::Read(weak_ptr<WritableDump> dump, istream &stream)
{
    auto node = new IndexLeafNode<TKey, TValue>(dump);

    // DumpObjectKind is assumed to be already read by IndexNode::Read

    uint16_t count = DumpTraits<uint16_t>::Read(stream);

    vector<TKey> keys;

    for (int i = 0; i < count; i++)
    {
        keys.push_back(DumpTraits<TKey>::Read(stream));
    }

    for (int i = 0; i < count; i++)
    {
        node->indexMap.insert(pair<TKey, TValue>(keys[i], DumpTraits<TValue>::Read(stream)));
    }

    return unique_ptr<IndexNode<TKey, TValue>>(node);
}

template<typename TKey, typename TValue>
void IndexLeafNode<TKey, TValue>::WriteInternal()
{
    WriteValue((uint8_t)DumpObjectKind::IndexLeafNode);
    WriteValue((uint16_t)indexMap.size());

	for (auto pair : indexMap)
    {
		WriteValue(pair.first);
    }

	for (auto pair : indexMap)
    {
		WriteValue(pair.second);
    }
}

template<typename TKey, typename TValue>
uint32_t IndexLeafNode<TKey, TValue>::NewLength() const
{
    return DumpTraits<uint8_t>::DumpSize((uint8_t)DumpObjectKind::IndexLeafNode)
        + DumpTraits<uint16_t>::DumpSize(indexMap.size())
        + Size * (DumpTraits<TKey>::DumpSize() + DumpTraits<TValue>::DumpSize());
}

template<typename TKey, typename TValue>
shared_ptr<IndexNodeIterator<TKey, TValue>> IndexLeafNode<TKey, TValue>::begin() const
{
    return shared_ptr<IndexNodeIterator<TKey, TValue>>(new IndexLeafIterator<TKey, TValue>(indexMap.begin()));
}

template<typename TKey, typename TValue>
shared_ptr<IndexNodeIterator<TKey, TValue>> IndexLeafNode<TKey, TValue>::end() const
{
    return shared_ptr<IndexNodeIterator<TKey, TValue>>(new IndexLeafIterator<TKey, TValue>(indexMap.end()));
}
