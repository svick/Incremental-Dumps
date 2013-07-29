#include <vector>
#include "IndexInnerIterator.h"

using std::vector;

template<typename TKey, typename TValue>
IndexInnerIterator<TKey, TValue>::IndexInnerIterator(
    IndexInnerNode<TKey, TValue> *node, uint16_t i, std::unique_ptr<IndexNodeIterator<TKey, TValue>> childIterator)
    : node(node), i(i), childIterator(std::move(childIterator))
{}

template<typename TKey, typename TValue>
IndexInnerIterator<TKey, TValue>::IndexInnerIterator(IndexInnerNode<TKey, TValue> *node, bool isBegin)
    : node(node),
      i(isBegin ? 0 : node->childOffsets.size()),
      childIterator(isBegin ? node->GetChildByIndex(0)->begin() : nullptr)
{}

template<typename TKey, typename TValue>
const pair<TKey, TValue> IndexInnerIterator<TKey, TValue>::operator *() const
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
        new IndexInnerIterator<TKey, TValue>(
            node, i, childIterator == nullptr ? nullptr : childIterator->Clone()));
}