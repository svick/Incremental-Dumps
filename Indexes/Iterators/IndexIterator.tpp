#include "IndexIterator.h"

template<typename TKey, typename TValue>
IndexIterator<TKey, TValue>::IndexIterator(std::unique_ptr<IndexNodeIterator<TKey, TValue>> nodeIterator)
    : nodeIterator(std::move(nodeIterator)), recentIncrements(0)
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
const std::pair<TKey, TValue> IndexIterator<TKey, TValue>::operator *() const
{
    return **nodeIterator;
}

template<typename TKey, typename TValue>
const std::unique_ptr<std::pair<TKey, TValue>> IndexIterator<TKey, TValue>::operator ->() const
{
    auto result = **nodeIterator;
    return std::unique_ptr<std::pair<TKey, TValue>>(new std::pair<TKey, TValue>(result));
}

template<typename TKey, typename TValue>
IndexIterator<TKey, TValue>& IndexIterator<TKey, TValue>::operator ++()
{
    ++(*nodeIterator);

    recentIncrements++;

    if (recentIncrements >= 10000)
    {
        nodeIterator->ClearNodeCacheIfTooBig();
        recentIncrements = 0;
    }

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
