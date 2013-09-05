#pragma once

#include <utility>
#include "../../DumpObjects/DumpObject.h"

template<typename TKey, typename TValue>
class IndexNodeIterator
{
public:
    virtual const std::pair<TKey, TValue> operator *() const = 0;
    virtual IndexNodeIterator& operator ++() = 0;

    virtual bool Equals(const IndexNodeIterator *other) const = 0;
    virtual std::unique_ptr<IndexNodeIterator> Clone() const = 0;

    virtual ~IndexNodeIterator() {};
};