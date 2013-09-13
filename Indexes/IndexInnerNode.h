#pragma once

#include <map>
#include "IndexNode.h"

template<typename TKey, typename TValue>
class IndexInnerNode : public IndexNode<TKey, TValue>
{
    template<typename TIndexKey, typename TIndexValue>
    friend class IndexInnerIterator;

    typedef typename IndexNode<TKey, TValue>::SplitResult SplitResult;

    using DumpObjectBase::WriteValue;
private:
    std::vector<TKey> keys;
    std::vector<Offset> childOffsets;
    std::vector<std::unique_ptr<IndexNode<TKey, TValue>>> cachedChildren;

    IndexNode<TKey, TValue>* GetChildByIndex(std::uint16_t index);
    std::uint16_t GetKeyIndex(TKey key);

    void AfterAdd(std::uint16_t updatedChildIndex);

protected:
    virtual void WriteInternal() OVERRIDE;
public:
    static std::unique_ptr<IndexNode<TKey, TValue>> Read(std::weak_ptr<WritableDump> dump, std::istream &stream);

    IndexInnerNode(std::weak_ptr<WritableDump> dump);
    IndexInnerNode(std::weak_ptr<WritableDump> dump, SplitResult splitResult);

    virtual void Write() OVERRIDE;

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

#include "IndexInnerNode.tpp"
