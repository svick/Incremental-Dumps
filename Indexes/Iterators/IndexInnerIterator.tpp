#include <vector>
#include "IndexInnerIterator.h"

template<typename TKey, typename TValue>
IndexInnerIterator<TKey, TValue>::IndexInnerIterator(IndexInnerNode<TKey, TValue> *node, bool isBegin)
    : node(node),
      i(isBegin ? 0 : node->childOffsets.size()),
      childIterator(isBegin ? node->GetChildByIndex(0)->begin() : nullptr)
{
    node->iterators++;
}

template<typename TKey, typename TValue>
IndexInnerIterator<TKey, TValue>::IndexInnerIterator(const IndexInnerIterator<TKey, TValue>& other)
    : node(other.node), i(other.i), childIterator(other.childIterator == nullptr ? nullptr : other.childIterator->Clone())
{
    node->iterators++;
}

template<typename TKey, typename TValue>
const std::pair<TKey, TValue> IndexInnerIterator<TKey, TValue>::operator *() const
{
    return **childIterator;
}

template<typename TKey, typename TValue>
IndexInnerIterator<TKey, TValue>& IndexInnerIterator<TKey, TValue>::operator ++()
{
    ++(*childIterator);

    while (childIterator != nullptr && childIterator->Equals(node->GetChildByIndex(i)->end().get()))
    {
        i++;

        if (i == node->childOffsets.size())
            childIterator = nullptr;
        else
            childIterator = node->GetChildByIndex(i)->begin();
    }

    return *this;
}

template<typename TKey, typename TValue>
bool IndexInnerIterator<TKey, TValue>::Equals(const IndexNodeIterator<TKey, TValue> *other) const
{
    auto otherCasted = dynamic_cast<const IndexInnerIterator<TKey, TValue>*>(other);
    if (otherCasted == nullptr)
        return false;

    if (i == otherCasted->i)
    {
        if (i == node->childOffsets.size())
            return true;

        if (childIterator->Equals(otherCasted->childIterator.get()))
            return true;
    }

    return false;
}

template<typename TKey, typename TValue>
std::unique_ptr<IndexNodeIterator<TKey, TValue>> IndexInnerIterator<TKey, TValue>::Clone() const
{
    return std::unique_ptr<IndexNodeIterator<TKey, TValue>>(
        new IndexInnerIterator<TKey, TValue>(*this));
}

template<typename TKey, typename TValue>
void IndexInnerIterator<TKey, TValue>::ClearNodeCacheIfTooBig()
{
    if (node->NodesCount() >= 5000)
        node->ClearCached();
}

template<typename TKey, typename TValue>
IndexInnerIterator<TKey, TValue>::~IndexInnerIterator()
{
    node->iterators--;
}