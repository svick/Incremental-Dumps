#pragma once

#include <utility>
#include <memory>
#include <iterator>
#include "IndexNodeIterator.h"

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

#include "IndexIterator.tpp"