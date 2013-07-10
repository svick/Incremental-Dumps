#include <utility>
#include <vector>
#include "../DumpObjects/DumpTraits.h"
#include "Iterators/IndexLeafIterator.h"

using std::pair;
using std::vector;

template<typename TKey, typename TValue>
TValue IndexLeafNode<TKey, TValue>::Get(TKey key)
{
    auto found = map.find(key);
    if (found == map.end())
        return TValue();
    return found->second;
}

template<typename TKey, typename TValue>
void IndexLeafNode<TKey, TValue>::Add(TKey key, TValue value)
{
    if (map.size() >= Size)
    {
        // TODO
        throw new DumpException();
    }

    map.insert(pair<TKey, TValue>(key, value));
    Write();
}

template<typename TKey, typename TValue>
void IndexLeafNode<TKey, TValue>::AddOrUpdate(TKey key, TValue value)
{
    auto pos = map.find(key);
    if (pos == map.end())
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
    map.erase(key);
    Write();
}

template<typename TKey, typename TValue>
IndexLeafNode<TKey, TValue>::IndexLeafNode(weak_ptr<WritableDump> dump)
    : IndexNode(dump)
{
}

template<typename TKey, typename TValue>
unique_ptr<IndexNode<TKey, TValue>> IndexLeafNode<TKey, TValue>::Read(weak_ptr<WritableDump> dump, istream &stream)
{
    auto node = new IndexLeafNode<TKey, TValue>(dump);

    uint8_t count = DumpTraits<uint8_t>::Read(stream);

    vector<TKey> keys;

    for (int i = 0; i < count; i++)
    {
        keys.push_back(DumpTraits<TKey>::Read(stream));
    }

    for (int i = 0; i < count; i++)
    {
        node->map.insert(pair<TKey, TValue>(keys[i], DumpTraits<TValue>::Read(stream)));
    }

    return unique_ptr<IndexNode<TKey, TValue>>(node);
}

template<typename TKey, typename TValue>
void IndexLeafNode<TKey, TValue>::WriteInternal()
{
    WriteValue((uint8_t)DumpObjectKind::IndexLeafNode);
    WriteValue((uint8_t)map.size());

	for (auto pair : map)
    {
		WriteValue(pair.first);
    }

	for (auto pair : map)
    {
		WriteValue(pair.second);
    }
}

template<typename TKey, typename TValue>
uint32_t IndexLeafNode<TKey, TValue>::NewLength() const
{
    return 2 * DumpTraits<uint8_t>::DumpSize()
        + Size * (DumpTraits<TKey>::DumpSize() + DumpTraits<TValue>::DumpSize());
}

template<typename TKey, typename TValue>
shared_ptr<IndexNodeIterator<TKey, TValue>> IndexLeafNode<TKey, TValue>::begin() const
{
    return shared_ptr<IndexNodeIterator<TKey, TValue>>(new IndexLeafIterator<TKey, TValue>(map.begin()));
}

template<typename TKey, typename TValue>
shared_ptr<IndexNodeIterator<TKey, TValue>> IndexLeafNode<TKey, TValue>::end() const
{
    return shared_ptr<IndexNodeIterator<TKey, TValue>>(new IndexLeafIterator<TKey, TValue>(map.end()));
}