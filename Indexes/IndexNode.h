#pragma once

#include "../DumpObjects/DumpObject.h"

using std::shared_ptr;

template<typename TKey, typename TValue>
class IndexNodeIterator;

template<typename TKey, typename TValue>
class IndexNode;

template<typename TKey, typename TValue>
class IndexNode : public DumpObject
{
protected:
    static const int Size = 4096;

    bool modified;

    unsigned iterators;

    virtual void WriteInternal() = 0;
public:
    struct SplitResult
    {
        SplitResult(
            std::unique_ptr<IndexNode<TKey, TValue>> leftNode,
            std::unique_ptr<IndexNode<TKey, TValue>> rightNode,
            TKey splitKey)
            : LeftNode(std::move(leftNode)), RightNode(std::move(rightNode)), SplitKey(splitKey)
        {}

        SplitResult(SplitResult&& other)
            : LeftNode(std::move(other.LeftNode)), RightNode(std::move(other.RightNode)), SplitKey(std::move(other.SplitKey))
        {}

        std::unique_ptr<IndexNode<TKey, TValue>> LeftNode;
        std::unique_ptr<IndexNode<TKey, TValue>> RightNode;
        TKey SplitKey;
    };

    static unique_ptr<IndexNode> Read(std::weak_ptr<WritableDump> dump, std::uint64_t offset);
    static unique_ptr<IndexNode> CreateNew(std::weak_ptr<WritableDump> dump);

    IndexNode(std::weak_ptr<WritableDump> dump);

    virtual void Write() override;
    void Write(bool force);

    virtual TValue Get(TKey key) = 0;
    virtual void Add(TKey key, TValue value) = 0;
    virtual void AddOrUpdate(TKey key, TValue value) = 0;
    virtual void Remove(TKey key) = 0;

    bool IsOversized();
    virtual std::uint32_t RealLength() = 0;
    virtual SplitResult Split() = 0;

    bool CanBeCleared();
    virtual std::uint32_t NodesCount() = 0;
    void ClearCached();
    virtual void ClearCachedInternal() = 0;

    virtual unique_ptr<IndexNodeIterator<TKey, TValue>> begin() = 0;
    virtual unique_ptr<IndexNodeIterator<TKey, TValue>> end() = 0;
};

#include "IndexNode.tpp"
