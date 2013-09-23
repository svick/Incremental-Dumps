#pragma once

#include <utility>
#include "../DumpObjects/Offset.h"
#include "Iterators/IndexIterator.h"
#include "IndexNode.h"

using std::pair;
using std::shared_ptr;

/** @file */

/**
 * @brief An index that maps between two types.
 *
 * It is implemented as a B-tree.
 */
template<typename TKey, typename TValue>
class Index
{
private:
    bool rootNodeUnsaved;
    unique_ptr<IndexNode<TKey, TValue>> rootNode;
    std::weak_ptr<WritableDump> dump;
    std::weak_ptr<Offset> fileHeaderOffset;

    int recentAccesses;

    void AfterAdd();
    void AfterAccess();
public:
    /**
     * @param fileHeaderOffset Pointer to field that stores the offset to the root node of this index.
     * @param delaySave If @c true, the root node of this index will be saved only when necessary.
     *                  This is required for ReadableDump::spaceIndex, because of cyclic dependency between it and SpaceManager.
     *                  @c true is not the only option, so that most indexes can have their root nodes at the start of the file.
     */
    Index(std::weak_ptr<WritableDump> dump, Offset* fileHeaderOffset, bool delaySave = false);

    /**
    * Returns the value for the given @a key.
    */
    TValue Get(TKey key);
    /**
    * Adds the given @a key-@a value pair to the index.
    *
    * It is assumed that @a key is not in the index yet.
    */
    void Add(TKey key, TValue value);
    /**
     * Sets the @a value for the given @a key.
     *
     * Works whether @a key is already in the index or not.
     */
    void AddOrUpdate(TKey key, TValue value);
    /**
     * Removes the given @a key from the index.
     */
    void Remove(TKey key);

    /**
     * Returns forward input iterator that points at the first item in the index.
     */
    IndexIterator<TKey, TValue> begin() const;
    /**
     * Returns iterator that points past the last item in the index.
     */
    IndexIterator<TKey, TValue> end() const;

    /**
     * Writes the index to the disk.
     */
    void Write();
};

/**
 * Returns a key that can be used to insert new item to the index.
 *
 * This is done by (inefficiently) finding the max key in the index and returning that value plus one.
 */
template<typename TIndex>
auto getNewId(const TIndex &index) -> decltype(index.begin()->first)
{
    // pair's default ordering will work fine here
    auto maxPairRef = std::max_element(index.begin(), index.end());
    decltype(index.begin()->first) newId;
    if (maxPairRef == index.end())
        newId = 1;
    else
        newId = maxPairRef->first + 1;

    return newId;
}

#include "Index.tpp"