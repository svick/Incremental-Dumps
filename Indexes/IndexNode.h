#pragma once

#include "../DumpObjects/DumpObject.h"

using std::shared_ptr;

template<typename TKey, typename TValue>
class IndexNodeIterator;

template<typename TKey, typename TValue>
class IndexNode;

/**
 * Abstract base class for node in the B-tree of an Index.
 */
template<typename TKey, typename TValue>
class IndexNode : public DumpObject
{
protected:
    /**
     * The size of index node on the disk in bytes.
     * Nodes smaller than this are saved with this size, to avoid reallocation for each change.
     * Nodes larger than this are Split() in two.
     */
    static const int Size = 4096;

    /**
     * Whether the node has been modified since last Write().
     */
    bool modified;

    /**
     * The number of iterators that currently exist for this node.
     *
     * Used to make sure nodes with iterators are not cleared from cache.
     */
    unsigned iterators;

    virtual void WriteInternal() = 0;
public:
    /**
     * Contains results of performing Split().
     */
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

        /**
         * The left part of the original node as a new node.
         */
        std::unique_ptr<IndexNode<TKey, TValue>> LeftNode;
        /**
         * The right part of the original node as a new node.
         */
        std::unique_ptr<IndexNode<TKey, TValue>> RightNode;
        /**
         * The key by which the node was split.
         */
        TKey SplitKey;
    };

    /**
     * Reads node from the given @a offset.
     */
    static unique_ptr<IndexNode> Read(std::weak_ptr<WritableDump> dump, std::uint64_t offset);
    /**
     * Creates and returns new IndexLeafNode that can be used as the root node for empty index.
     */
    static unique_ptr<IndexNode> CreateNew(std::weak_ptr<WritableDump> dump);

    IndexNode(std::weak_ptr<WritableDump> dump);

    virtual void Write() OVERRIDE;
    /**
     * @param force If set to @c true, the node is written to the disk even if #modified is @c false.
     */
    void Write(bool force);

    virtual TValue Get(TKey key) = 0;
    virtual void Add(TKey key, TValue value) = 0;
    virtual void AddOrUpdate(TKey key, TValue value) = 0;
    virtual void Remove(TKey key) = 0;

    /**
     * Returns @c true if this node wouldn't fit into Size if written on disk.
     */
    bool IsOversized();
    /**
     * How many bytes are actually required for writing this node on disk.
     *
     * This is based on the actual number and size of elements in the node, not on Size.
     */
    virtual std::uint32_t RealLength() = 0;
    /**
     * Splits this node into two new nodes.
     * Each of them gets half of items from this node.
     */
    virtual SplitResult Split() = 0;

    /**
     * Whether this node is not in use, which means it can be cleared from cache.
     */
    bool CanBeCleared();
    /**
     * How many nodes are there in total in the subtree starting from this node.
     */
    virtual std::uint32_t NodesCount() = 0;
    /**
     * Clears the cache of child nodes of this node.
     * Makes sure the cleared nodes are written to the disk first
     * and that nodes that are in use by iterators are not cleared.
     */
    void ClearCached();
    /**
     * Actually performs clearing the cache.
     *
     * Has to be public, so that nodes can access this method of their children.
     */
    virtual void ClearCachedInternal() = 0;

    virtual unique_ptr<IndexNodeIterator<TKey, TValue>> begin() = 0;
    virtual unique_ptr<IndexNodeIterator<TKey, TValue>> end() = 0;
};

#include "IndexNode.tpp"
