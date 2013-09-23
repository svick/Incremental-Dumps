#pragma once

#include <utility>
#include "IndexNodeIterator.h"

/**
 * Iterator for IndexInnerNode.
 *
 * Works on two levels:
 * 1. Iterating over the IndexNodeIterator for a child node.
 * 2. Iterating over the child nodes of #node.
 *
 * When level 1 reaches end, level 2 is incremented and
 * level 1 is then restarted for a new child node.
 */
template<typename TKey, typename TValue>
class IndexInnerIterator : public IndexNodeIterator<TKey, TValue>
{
    template<typename TIndexKey, typename TIndexValue>
    friend class IndexInnerNode;
private:
    /**
     * The node this iterator iterates
     */
    IndexInnerNode<TKey, TValue> *node;
    /**
     * The index of the child node of #node that's currently being iterated (level 2).
     */
    uint16_t i;
    /**
     * The iterator for the current child node (level 1).
     */
    std::unique_ptr<IndexNodeIterator<TKey, TValue>> childIterator;

    IndexInnerIterator(IndexInnerNode<TKey, TValue> *node, bool isBegin);
    IndexInnerIterator(const IndexInnerIterator<TKey, TValue>& other);
public:
    virtual const std::pair<TKey, TValue> operator *() const OVERRIDE;
    virtual IndexInnerIterator& operator ++() OVERRIDE;
    virtual bool Equals(const IndexNodeIterator<TKey, TValue> *other) const OVERRIDE;
    virtual std::unique_ptr<IndexNodeIterator<TKey, TValue>> Clone() const OVERRIDE;

    virtual void ClearNodeCacheIfTooBig() OVERRIDE;

    virtual ~IndexInnerIterator();
};

#include "IndexInnerIterator.tpp"