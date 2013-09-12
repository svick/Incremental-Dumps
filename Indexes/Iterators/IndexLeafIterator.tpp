#include <vector>
#include "IndexLeafIterator.h"

template<typename TKey, typename TValue>
IndexLeafIterator<TKey, TValue>::IndexLeafIterator(IndexLeafNode<TKey, TValue> *node, MapIterator mapIterator)
    : node(node), mapIterator(mapIterator)
{
    node->iterators++;
}

template<typename TKey, typename TValue>
IndexLeafIterator<TKey, TValue>::IndexLeafIterator(const IndexLeafIterator<TKey, TValue>& other)
    : node(other.node), mapIterator(other.mapIterator)
{
    node->iterators++;
}

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
bool IndexLeafIterator<TKey, TValue>::Equals(const IndexNodeIterator<TKey, TValue> *other) const
{
    auto otherCasted = dynamic_cast<const IndexLeafIterator<TKey, TValue>*>(other);
    if (otherCasted == nullptr)
        return false;

    return mapIterator == otherCasted->mapIterator;
}

template<typename TKey, typename TValue>
std::unique_ptr<IndexNodeIterator<TKey, TValue>> IndexLeafIterator<TKey, TValue>::Clone() const
{
    return std::unique_ptr<IndexNodeIterator<TKey, TValue>>(new IndexLeafIterator<TKey, TValue>(*this));
}

template<typename TKey, typename TValue>
void IndexLeafIterator<TKey, TValue>::ClearNodeCacheIfTooBig()
{}

template<typename TKey, typename TValue>
IndexLeafIterator<TKey, TValue>::~IndexLeafIterator()
{
    node->iterators--;
}