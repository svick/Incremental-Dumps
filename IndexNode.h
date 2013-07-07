#pragma once

#include <utility>
#include "DumpObject.h"

using std::pair;
using std::shared_ptr;

template<typename TKey, typename TValue>
class IndexNodeIterator
{
public:
    virtual const pair<TKey, TValue> operator *() const = 0;
    virtual IndexNodeIterator& operator ++() = 0;
    virtual bool equals(const IndexNodeIterator *other) const = 0;
};

template<typename TKey, typename TValue>
class IndexNode : public DumpObject
{
protected:
    virtual void Write(ostream &stream) = 0;

    enum class NodeKind : char
    {
        LeafNode = 1
    };

public:
    static unique_ptr<IndexNode> Read(weak_ptr<WritableDump> dump, int64_t offset);
    static unique_ptr<IndexNode> CreateNew(weak_ptr<WritableDump> dump);

    IndexNode(weak_ptr<WritableDump> dump);

    using DumpObject::Write;

    virtual TValue operator[](TKey key) = 0;
    virtual void Add(TKey key, TValue value) = 0;
    virtual void Remove(TKey key) = 0;

    virtual shared_ptr<IndexNodeIterator<TKey, TValue>> begin() const = 0;
    virtual shared_ptr<IndexNodeIterator<TKey, TValue>> end() const = 0;
};

#include "IndexNode.hpp"