#include "IndexIterator.h"

using std::move;

template<typename TKey, typename TValue>
IndexIterator<TKey, TValue>::IndexIterator(shared_ptr<IndexNodeIterator<TKey, TValue>> nodeIterator)
    : nodeIterator(nodeIterator)
{}

template<typename TKey, typename TValue>
const pair<TKey, TValue> IndexIterator<TKey, TValue>::operator *() const
{
    return **nodeIterator;
}

template<typename TKey, typename TValue>
IndexIterator<TKey, TValue>& IndexIterator<TKey, TValue>::operator ++()
{
    ++(*nodeIterator);
    return *this;
}

template<typename TKey, typename TValue>
bool IndexIterator<TKey, TValue>::operator ==(const IndexIterator<TKey, TValue> other)
{
    return nodeIterator->equals(other.nodeIterator.get());
}

template<typename TKey, typename TValue>
bool IndexIterator<TKey, TValue>::operator !=(const IndexIterator<TKey, TValue> other)
{
    return !(*this == other);
}