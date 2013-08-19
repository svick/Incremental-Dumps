#pragma once

#include <map>
#include "IndexNode.h"

using std::map;

template<typename TKey, typename TValue>
class IndexLeafNode : public IndexNode<TKey, TValue>
{
    using typename IndexNode<TKey, TValue>::SplitResult;
private:
    map<TKey, TValue> indexMap;
protected:
    virtual void WriteInternal() override;
public:
    static unique_ptr<IndexNode<TKey, TValue>> Read(std::weak_ptr<WritableDump> dump, std::istream &stream);

    IndexLeafNode(std::weak_ptr<WritableDump> dump);

    using DumpObject::Write;
    using DumpObjectBase::WriteValue;
    using DumpObjectBase::ValueSize;

    virtual uint32_t NewLength() override;

    virtual TValue Get(TKey key) override;
    virtual void Add(TKey key, TValue value) override;
    virtual void AddOrUpdate(TKey key, TValue value) override;
    virtual void Remove(TKey key) override;

    virtual std::uint32_t RealLength() override;
    virtual SplitResult Split() override;

    virtual unique_ptr<IndexNodeIterator<TKey, TValue>> begin() override;
    virtual unique_ptr<IndexNodeIterator<TKey, TValue>> end() override;
};

#include "IndexLeafNode.tpp"
