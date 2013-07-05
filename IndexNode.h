#pragma once

#include "DumpObject.h"

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
    // TODO:
    //virtual void Remove(TKey key);
    //virtual void Remove(TKey key, TValue value); // required for memory index
};

#include "IndexNode.hpp"