#pragma once

#include <map>
#include "IndexNode.h"

using std::map;

template<typename TKey, typename TValue>
class IndexLeafNode : public IndexNode<TKey, TValue>
{
private:
    static const int Size = 0xFFFF;

    map<TKey, TValue> indexMap;
protected:
    virtual void WriteInternal() override;
public:
    static unique_ptr<IndexNode<TKey, TValue>> Read(weak_ptr<WritableDump> dump, istream &stream);

    IndexLeafNode(weak_ptr<WritableDump> dump);

    using DumpObject::Write;
    using DumpObjectBase::WriteValue;

    virtual uint32_t NewLength() override;

    virtual TValue Get(TKey key) override;
    virtual void Add(TKey key, TValue value) override;
    virtual void AddOrUpdate(TKey key, TValue value) override;
    virtual void Remove(TKey key) override;

    virtual shared_ptr<IndexNodeIterator<TKey, TValue>> begin() const override;
    virtual shared_ptr<IndexNodeIterator<TKey, TValue>> end() const override;
};

#include "IndexLeafNode.tpp"
