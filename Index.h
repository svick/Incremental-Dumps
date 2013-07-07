#pragma once

#include <utility>
#include <iterator>
#include "Offset.h"
#include "IndexNode.h"

using std::pair;
using std::shared_ptr;
using std::iterator;
using std::input_iterator_tag;

template<typename TKey, typename TValue>
class IndexIterator : public iterator<input_iterator_tag, const pair<TKey, TValue>, int32_t>
{
    template<typename TKey, typename TValue>
    friend class Index;
private:
    shared_ptr<IndexNodeIterator<TKey, TValue>> nodeIterator;
    IndexIterator(shared_ptr<IndexNodeIterator<TKey, TValue>> nodeIterator);
public:
    const pair<TKey, TValue> operator *() const;
    IndexIterator<TKey, TValue>& operator ++();
    bool operator ==(const IndexIterator<TKey, TValue> other);
    bool operator !=(const IndexIterator<TKey, TValue> other);
};

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

#include "Index.hpp"