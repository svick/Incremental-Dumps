#pragma once

#include <utility>
#include "../DumpObjects/Offset.h"
#include "Iterators/IndexIterator.h"
#include "IndexNode.h"

using std::pair;
using std::shared_ptr;

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
    // fileHeaderOffset is assumed to be part of dump
    Index(std::weak_ptr<WritableDump> dump, Offset* fileHeaderOffset, bool delaySave = false);

    TValue Get(TKey key);
    void Add(TKey key, TValue value);
    void AddOrUpdate(TKey key, TValue value);
    void Remove(TKey key);

    IndexIterator<TKey, TValue> begin() const;
    IndexIterator<TKey, TValue> end() const;

    void Write();
};

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