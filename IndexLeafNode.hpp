#include <utility>
#include "DumpTraits.h"

using std::pair;

template<typename TKey, typename TValue>
TValue IndexLeafNode<TKey, TValue>::operator[](TKey key)
{
    return map.find(key)->second;
}

template<typename TKey, typename TValue>
void IndexLeafNode<TKey, TValue>::Add(TKey key, TValue value)
{
    map.insert(pair<TKey, TValue>(key, value));
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

    unique_ptr<TKey[]> keys(new TKey[count]);

    for (int i = 0; i < count; i++)
    {
        keys[i] = DumpTraits<TKey>::Read(stream);
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
int64_t IndexLeafNode<TKey, TValue>::NewLength()
{
    return DumpTraits<char>::DumpSize()
        + map.size() * (DumpTraits<TKey>::DumpSize() + DumpTraits<TValue>::DumpSize());
}