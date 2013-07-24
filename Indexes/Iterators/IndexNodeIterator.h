#pragma once

#include <utility>
#include "../../DumpObjects/DumpObject.h"

template<typename TKey, typename TValue>
class IndexNodeIterator
{
public:
    virtual const std::pair<TKey, TValue> operator *() const = 0;
    virtual IndexNodeIterator& operator ++() = 0;
    virtual bool equals(const IndexNodeIterator *other) const = 0;
};