#pragma once

#include <utility>
#include <map>
#include "IndexNode.h"

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

template<typename TKey, typename TValue>
class IndexLeafNode : public IndexNode<TKey, TValue>
{
private:
    static const int Size = 10; // has to be at most 128 for now

    map<TKey, TValue> map;
protected:
    virtual void Write(ostream &stream);
public:
    static unique_ptr<IndexNode> Read(weak_ptr<WritableDump> dump, istream &stream);

    IndexLeafNode(weak_ptr<WritableDump> dump);

    using DumpObject::Write;
    virtual int32_t NewLength();

    virtual TValue operator[](TKey key);
    virtual void Add(TKey key, TValue value);
    virtual void Remove(TKey key);

    virtual shared_ptr<IndexNodeIterator<TKey, TValue>> begin() const;
    virtual shared_ptr<IndexNodeIterator<TKey, TValue>> end() const;
};

#include "IndexLeafNode.hpp"