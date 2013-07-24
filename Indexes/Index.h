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
    weak_ptr<WritableDump> dump;
    weak_ptr<Offset> fileHeaderOffset;

    void AfterAdd();
public:
    Index(weak_ptr<WritableDump> dump, weak_ptr<Offset> fileHeaderOffset, bool delaySave = false);

    TValue Get(TKey key);
    void Add(TKey key, TValue value);
    void AddOrUpdate(TKey key, TValue value);
    void Remove(TKey key);

    IndexIterator<TKey, TValue> begin() const;
    IndexIterator<TKey, TValue> end() const;

    void Write();
};

#include "Index.tpp"
