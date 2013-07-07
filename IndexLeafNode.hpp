#include <utility>
#include <vector>
#include "DumpTraits.h"

using std::pair;
using std::vector;

template<typename TKey, typename TValue>
IndexLeafIterator<TKey, TValue>::IndexLeafIterator(MapIterator mapIterator)
    : mapIterator(mapIterator)
{}

template<typename TKey, typename TValue>
const pair<TKey, TValue> IndexLeafIterator<TKey, TValue>::operator *() const
{
    return *mapIterator;
}

template<typename TKey, typename TValue>
IndexLeafIterator<TKey, TValue>& IndexLeafIterator<TKey, TValue>::operator ++()
{
    ++mapIterator;
    return *this;
}

template<typename TKey, typename TValue>
bool IndexLeafIterator<TKey, TValue>::equals(const IndexNodeIterator<TKey, TValue> *other) const
{
    auto otherCasted = dynamic_cast<const IndexLeafIterator<TKey, TValue>*>(other);
    if (otherCasted == nullptr)
        return false;

    return mapIterator == otherCasted->mapIterator;
}

template<typename TKey, typename TValue>
TValue IndexLeafNode<TKey, TValue>::operator[](TKey key)
{
    return map.find(key)->second;
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

    char count = DumpTraits<char>::Read(stream);

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
void IndexLeafNode<TKey, TValue>::Write(ostream &stream)
{
    DumpTraits<char>::Write(stream, (char)NodeKind::LeafNode);
	DumpTraits<char>::Write(stream, map.size());

	for (auto pair : map)
    {
		DumpTraits<TKey>::Write(stream, pair.first);
    }

	for (auto pair : map)
    {
		DumpTraits<TValue>::Write(stream, pair.second);
    }
}

template<typename TKey, typename TValue>
int32_t IndexLeafNode<TKey, TValue>::NewLength()
{
    return 2 * DumpTraits<char>::DumpSize()
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