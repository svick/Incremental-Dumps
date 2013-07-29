#pragma once

#include <utility>

template<typename TKey, typename TValue>
class IndexInnerIterator : public IndexNodeIterator<TKey, TValue>
{
    template<typename TIndexKey, typename TIndexValue>
    friend class IndexInnerNode;
private:
    IndexInnerNode<TKey, TValue> *node;
    uint16_t i;
    std::unique_ptr<IndexNodeIterator<TKey, TValue>> childIterator;

    IndexInnerIterator(IndexInnerNode<TKey, TValue> *node, uint16_t i, std::unique_ptr<IndexNodeIterator<TKey, TValue>> childIterator);
    IndexInnerIterator(IndexInnerNode<TKey, TValue> *node, bool isBegin);
public:
    virtual const pair<TKey, TValue> operator *() const override;
    virtual IndexInnerIterator& operator ++() override;
    virtual bool Equals(const IndexNodeIterator<TKey, TValue> *other) const override;
    virtual std::unique_ptr<IndexNodeIterator<TKey, TValue>> Clone() const override;
};

#include "IndexInnerIterator.tpp"