#pragma once

#include <utility>
#include <map>

using std::pair;
using std::map;

template<typename TKey, typename TValue>
class IndexLeafIterator : public IndexNodeIterator<TKey, TValue>
{
    typedef typename map<TKey, TValue>::const_iterator MapIterator;

    template<typename TIndexKey, typename TIndexValue>
    friend class IndexLeafNode;
private:
    MapIterator mapIterator;

    IndexLeafIterator(MapIterator mapIterator);
public:
    virtual const pair<TKey, TValue> operator *() const override;
    virtual IndexLeafIterator& operator ++() override;
    virtual bool equals(const IndexNodeIterator<TKey, TValue> *other) const override;
};

#include "IndexLeafIterator.tpp"
