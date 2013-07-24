#include <vector>
#include "IndexInnerIterator.h"

using std::vector;

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

    while (childIterator == node->GetChildByIndex(i)->end())
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
bool IndexInnerIterator<TKey, TValue>::equals(const IndexNodeIterator<TKey, TValue> *other) const
{
    auto otherCasted = dynamic_cast<const IndexInnerIterator<TKey, TValue>*>(other);
    if (otherCasted == nullptr)
        return false;

    if (i == otherCasted->i)
    {
        if (i == node->childOffsets.size())
            return true;

        if (childIterator->equals(otherCasted->childIterator.get()))
            return true;
    }

    return false;
}