#pragma once

#include "../DumpObjects/DumpObject.h"

using std::shared_ptr;

template<typename TKey, typename TValue>
class IndexNodeIterator;

template<typename TKey, typename TValue>
class IndexNode : public DumpObject
{
protected:
    virtual void WriteInternal() = 0;

public:
    static unique_ptr<IndexNode> Read(weak_ptr<WritableDump> dump, uint64_t offset);
    static unique_ptr<IndexNode> CreateNew(weak_ptr<WritableDump> dump);

    IndexNode(weak_ptr<WritableDump> dump);

    using DumpObject::Write;

    virtual TValue Get(TKey key) = 0;
    virtual void Add(TKey key, TValue value) = 0;
    virtual void AddOrUpdate(TKey key, TValue value) = 0;
    virtual void Remove(TKey key) = 0;

    virtual shared_ptr<IndexNodeIterator<TKey, TValue>> begin() const = 0;
    virtual shared_ptr<IndexNodeIterator<TKey, TValue>> end() const = 0;
};

#include "IndexNode.tpp"