#pragma once

#include <utility>
#include "../../DumpObjects/DumpObject.h"

/**
 * Abstract base class for iterators for a node in the Index.
 */
template<typename TKey, typename TValue>
class IndexNodeIterator
{
public:
    virtual const std::pair<TKey, TValue> operator *() const = 0;
    virtual IndexNodeIterator& operator ++() = 0;

    /**
     * Compares this iterator with another for equality.
     *
     * @c operator== can't be used because of inheritance.
     */
    virtual bool Equals(const IndexNodeIterator *other) const = 0;
    /**
     * Returns a copy of this iterator.
     *
     * Copy constructor can't be used because of inheritance.
     */
    virtual std::unique_ptr<IndexNodeIterator> Clone() const = 0;

    /**
     * Clears the child node cache of the node this iterator iterates.
     */
    virtual void ClearNodeCacheIfTooBig() = 0;

    virtual ~IndexNodeIterator() {};
};