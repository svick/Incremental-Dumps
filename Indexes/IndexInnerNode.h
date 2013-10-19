#pragma once

#include <map>
#include "IndexNode.h"

/**
 * Inner node in the B-tree of an Index.
 *
 * Contains *n* #keys and *n*+1 #childOffsets.
 *
 * For all keys *x* in child node at position *i*, the following holds:
 *
 *     keys[i-1] <= x < keys[i]
 */
template<typename TKey, typename TValue>
class IndexInnerNode : public IndexNode<TKey, TValue>
{
    template<typename TIndexKey, typename TIndexValue>
    friend class IndexInnerIterator;

    typedef typename IndexNode<TKey, TValue>::SplitResult SplitResult;

    using DumpObjectBase::WriteValue;
private:
    /**
     * Contains keys for this node.
     */
    std::vector<TKey> keys;
    /**
     * Contains offsets where child nodes of this node are located.
     */
    std::vector<Offset> childOffsets;
    /**
     * Cache of child nodes that were recently accessed,
     * so that they don't have to be read and written to the disk over and over.
     */
    std::vector<std::unique_ptr<IndexNode<TKey, TValue>>> cachedChildren;

    /**
     * Creates a new empty inner node.
     */
    IndexInnerNode(std::weak_ptr<Dump> dump);

    /**
     * Returns pointer to child node at given @a index.
     * If the child node is not in #cachedChildren, it's loaded from disk.
     */
    IndexNode<TKey, TValue>* GetChildByIndex(std::uint16_t index);
    /**
     * Returns the index of the child node that could contain the value for the given @a key.
     * (The returned index has no relation with the Index type.)
     */
    std::uint16_t GetKeyIndex(TKey key);

    /**
     * Maintains invariants of B-tree after an update.
     *
     * This is done by splitting the updated node if it's too big.
     *
     * @param updatedChildIndex The index of the child node that was updated.
     */
    void AfterAdd(std::uint16_t updatedChildIndex);
protected:
    virtual void WriteInternal() OVERRIDE;
public:
    static std::unique_ptr<IndexNode<TKey, TValue>> Read(std::weak_ptr<Dump> dump, std::istream &stream);

    /**
     * Creates a new root node for an index,
     * its children are the results of splitting the former root node.
     */
    IndexInnerNode(std::weak_ptr<Dump> dump, SplitResult splitResult);

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
