#pragma once

#include <utility>
#include <map>

/**
 * Iterator for IndexLeafNode.
 *
 * Acts as a wrapper for iterator of @c std::map.
 */
template<typename TKey, typename TValue>
class IndexLeafIterator : public IndexNodeIterator<TKey, TValue>
{
    typedef typename std::map<TKey, TValue>::const_iterator MapIterator;

    template<typename TIndexKey, typename TIndexValue>
    friend class IndexLeafNode;
private:
    IndexLeafNode<TKey, TValue> *node;
    MapIterator mapIterator;

    IndexLeafIterator(IndexLeafNode<TKey, TValue> *node, MapIterator mapIterator);
    IndexLeafIterator(const IndexLeafIterator<TKey, TValue>& other);
public:
    virtual const std::pair<TKey, TValue> operator *() const OVERRIDE;
    virtual IndexLeafIterator& operator ++() OVERRIDE;
    virtual bool Equals(const IndexNodeIterator<TKey, TValue> *other) const OVERRIDE;
    virtual std::unique_ptr<IndexNodeIterator<TKey, TValue>> Clone() const OVERRIDE;

    virtual void ClearNodeCacheIfTooBig() OVERRIDE;

    virtual ~IndexLeafIterator();
};

#include "IndexLeafIterator.tpp"
