#pragma once

#include <map>
#include "IndexNode.h"

template<typename TKey, typename TValue>
class IndexInnerNode : public IndexNode<TKey, TValue>
{
    template<typename TIndexKey, typename TIndexValue>
    friend class IndexInnerIterator;

    using typename IndexNode<TKey, TValue>::SplitResult;

    using DumpObjectBase::WriteValue;
private:
    std::vector<TKey> keys;
    std::vector<Offset> childOffsets;
    std::vector<std::unique_ptr<IndexNode<TKey, TValue>>> cachedChildren;

    IndexNode<TKey, TValue>* GetChildByIndex(std::uint16_t index);
    std::uint16_t GetKeyIndex(TKey key);

    void AfterAdd(std::uint16_t updatedChildIndex);
protected:
    virtual void WriteInternal() override;
public:
    static std::unique_ptr<IndexNode<TKey, TValue>> Read(std::weak_ptr<WritableDump> dump, std::istream &stream);

    IndexInnerNode(std::weak_ptr<WritableDump> dump);
    IndexInnerNode(std::weak_ptr<WritableDump> dump, SplitResult splitResult);

    virtual void Write() override;

    virtual uint32_t NewLength() override;

    virtual TValue Get(TKey key) override;
    virtual void Add(TKey key, TValue value) override;
    virtual void AddOrUpdate(TKey key, TValue value) override;
    virtual void Remove(TKey key) override;

    virtual std::uint32_t RealLength() override;
    virtual SplitResult Split() override;

    virtual std::unique_ptr<IndexNodeIterator<TKey, TValue>> begin() override;
    virtual std::unique_ptr<IndexNodeIterator<TKey, TValue>> end() override;
};

#include "IndexInnerNode.tpp"
