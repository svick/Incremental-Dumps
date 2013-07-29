#include "IndexIterator.h"

using std::move;

template<typename TKey, typename TValue>
IndexIterator<TKey, TValue>::IndexIterator(unique_ptr<IndexNodeIterator<TKey, TValue>> nodeIterator)
    : nodeIterator(std::move(nodeIterator))
{}

template<typename TKey, typename TValue>
IndexIterator<TKey, TValue>::IndexIterator(const IndexIterator &other)
    : nodeIterator(other.nodeIterator->Clone())
{}

template<typename TKey, typename TValue>
IndexIterator<TKey, TValue>& IndexIterator<TKey, TValue>::operator =(const IndexIterator &other)
{
    nodeIterator = other.nodeIterator->Clone();

    return *this;
}

template<typename TKey, typename TValue>
const pair<TKey, TValue> IndexIterator<TKey, TValue>::operator *() const
{
    return **nodeIterator;
}

template<typename TKey, typename TValue>
const std::unique_ptr<pair<TKey, TValue>> IndexIterator<TKey, TValue>::operator ->() const
{
    auto result = **nodeIterator;
    return std::unique_ptr<pair<TKey, TValue>>(new pair<TKey, TValue>(result));
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
    return nodeIterator->Equals(other.nodeIterator.get());
}

template<typename TKey, typename TValue>
bool IndexIterator<TKey, TValue>::operator !=(const IndexIterator<TKey, TValue> other)
{
    return !(*this == other);
}
