#pragma once

#include <map>
#include "IndexNode.h"

template<typename TKey, typename TValue>
class IndexLeafNode : public IndexNode<TKey, TValue>
{
    template<typename TIndexKey, typename TIndexValue>
    friend class IndexLeafIterator;

    typedef typename IndexNode<TKey, TValue>::SplitResult SplitResult;

private:
    std::map<TKey, TValue> indexMap;
protected:
    virtual void WriteInternal() OVERRIDE;
public:
    static std::unique_ptr<IndexNode<TKey, TValue>> Read(std::weak_ptr<WritableDump> dump, std::istream &stream);

    IndexLeafNode(std::weak_ptr<WritableDump> dump);

    using DumpObject::Write;
    using DumpObjectBase::WriteValue;
    using DumpObjectBase::ValueSize;

    virtual uint32_t NewLength() OVERRIDE;

    virtual TValue Get(TKey key) OVERRIDE;
    virtual void Add(TKey key, TValue value) OVERRIDE;
    virtual void AddOrUpdate(TKey key, TValue value) OVERRIDE;
    virtual void Remove(TKey key) OVERRIDE;

    virtual std::uint32_t RealLength() OVERRIDE;
    virtual SplitResult Split() OVERRIDE;

    virtual std::uint32_t NodesCount() OVERRIDE;
    virtual void ClearCachedInternal() OVERRIDE;

    virtual std::unique_ptr<IndexNodeIterator<TKey, TValue>> begin() OVERRIDE;
    virtual std::unique_ptr<IndexNodeIterator<TKey, TValue>> end() OVERRIDE;
};

#include "IndexLeafNode.tpp"
