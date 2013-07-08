#pragma once

#include <utility>
#include "DumpObjects/Offset.h"
#include "Iterators/IndexIterator.h"
#include "IndexNode.h"

using std::pair;
using std::shared_ptr;

template<typename TKey, typename TValue>
class Index
{
private:
    bool fileHeaderZero;
    unique_ptr<IndexNode<TKey, TValue>> rootNode;
    weak_ptr<WritableDump> dump;
    weak_ptr<Offset> fileHeaderOffset;

    void Save();
public:
    Index(weak_ptr<WritableDump> dump, weak_ptr<Offset> fileHeaderOffset);

    TValue operator[](TKey key);
    void Add(TKey key, TValue value);
    void Remove(TKey key);

    IndexIterator<TKey, TValue> begin() const;
    IndexIterator<TKey, TValue> end() const;
};

#include "Index.tpp"