#pragma once

#include <utility>
#include <map>

using std::pair;
using std::map;

template<typename TKey, typename TValue>
class IndexLeafIterator : public IndexNodeIterator<TKey, TValue>
{
    typedef typename map<TKey, TValue>::const_iterator MapIterator;

    template<typename TKey, typename TValue>
    friend class IndexLeafNode;
private:
    MapIterator mapIterator;

    IndexLeafIterator(MapIterator mapIterator);
public:
    virtual const pair<TKey, TValue> operator *() const;
    virtual IndexLeafIterator& operator ++();
    virtual bool equals(const IndexNodeIterator *other) const;
};

#include "IndexLeafIterator.tpp"