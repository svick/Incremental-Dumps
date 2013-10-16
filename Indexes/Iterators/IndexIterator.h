#pragma once

#include <utility>
#include <memory>
#include <iterator>
#include "IndexNodeIterator.h"

/**
 * Forward input iterator of items in an Index.
 *
 * Dereferencing the iterator returns a pair containing key and the corresponding value.
 * The pairs are sorted by their key, ascending.
 *
 * This class exists to wrap pointer to IndexNodeIterator.
 * Pointer has to be used because of inheritance.
 */
template<typename TKey, typename TValue>
class IndexIterator : public std::iterator<std::input_iterator_tag, const std::pair<TKey, TValue>, std::int32_t>
{
    template<typename TIndexKey, typename TIndexValue>
    friend class Index;
private:
    std::unique_ptr<IndexNodeIterator<TKey, TValue>> nodeIterator;

    /**
     * How many increments were made recently.
     *
     * This counter is used to call IndexNodeIterator::ClearNodeCacheIfTooBig() from time to time.
     */
    std::uint32_t recentIncrements;

    IndexIterator(std::unique_ptr<IndexNodeIterator<TKey, TValue>> nodeIterator);
public:
    IndexIterator(const IndexIterator &other);
    IndexIterator& operator =(const IndexIterator &other);

    const std::pair<TKey, TValue> operator *() const;
    const std::unique_ptr<std::pair<TKey, TValue>> operator->() const;
    IndexIterator<TKey, TValue>& operator ++();
    bool operator ==(const IndexIterator<TKey, TValue> other);
    bool operator !=(const IndexIterator<TKey, TValue> other);
};

#include "IndexIterator.tpp"