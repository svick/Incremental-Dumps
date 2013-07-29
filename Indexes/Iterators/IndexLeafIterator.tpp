#include <vector>
#include "IndexLeafIterator.h"

using std::vector;

template<typename TKey, typename TValue>
IndexLeafIterator<TKey, TValue>::IndexLeafIterator(MapIterator mapIterator)
    : mapIterator(mapIterator)
{}

template<typename TKey, typename TValue>
const pair<TKey, TValue> IndexLeafIterator<TKey, TValue>::operator *() const
{
    return *mapIterator;
}

template<typename TKey, typename TValue>
IndexLeafIterator<TKey, TValue>& IndexLeafIterator<TKey, TValue>::operator ++()
{
    ++mapIterator;
    return *this;
}

template<typename TKey, typename TValue>
bool IndexLeafIterator<TKey, TValue>::Equals(const IndexNodeIterator<TKey, TValue> *other) const
{
    auto otherCasted = dynamic_cast<const IndexLeafIterator<TKey, TValue>*>(other);
    if (otherCasted == nullptr)
        return false;

    return mapIterator == otherCasted->mapIterator;
}

template<typename TKey, typename TValue>
std::unique_ptr<IndexNodeIterator<TKey, TValue>> IndexLeafIterator<TKey, TValue>::Clone() const
{
    return std::unique_ptr<IndexNodeIterator<TKey, TValue>>(new IndexLeafIterator<TKey, TValue>(*this));
}