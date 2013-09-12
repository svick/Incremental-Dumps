#pragma once

#include <map>
#include "IndexNode.h"

template<typename TKey, typename TValue>
class IndexLeafNode : public IndexNode<TKey, TValue>
{
    template<typename TIndexKey, typename TIndexValue>
    friend class IndexLeafIterator;

    using typename IndexNode<TKey, TValue>::SplitResult;
private:
    std::map<TKey, TValue> indexMap;
protected:
    virtual void WriteInternal() override;
public:
    static std::unique_ptr<IndexNode<TKey, TValue>> Read(std::weak_ptr<WritableDump> dump, std::istream &stream);

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

    virtual std::uint32_t NodesCount() override;
    virtual void ClearCachedInternal() override;

    virtual std::unique_ptr<IndexNodeIterator<TKey, TValue>> begin() override;
    virtual std::unique_ptr<IndexNodeIterator<TKey, TValue>> end() override;
};

#include "IndexLeafNode.tpp"
