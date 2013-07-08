#include "Index.h"

#include <memory>

using std::move;

template<typename TKey, typename TValue>
Index<TKey, TValue>::Index(weak_ptr<WritableDump> dump, weak_ptr<Offset> fileHeaderOffset)
    : dump(dump), fileHeaderOffset(fileHeaderOffset)
{
    auto offset = fileHeaderOffset.lock();

    if (offset->value == 0)
    {
        rootNode = IndexNode<TKey, TValue>::CreateNew(dump);
        fileHeaderZero = true;
    }
    else
        rootNode = IndexNode<TKey, TValue>::Read(dump, offset->value);
}

template<typename TKey, typename TValue>
TValue Index<TKey, TValue>::operator[](TKey key)
{
    return (*rootNode)[key];
}

template<typename TKey, typename TValue>
void Index<TKey, TValue>::Add(TKey key, TValue value)
{
    rootNode->Add(key, value);

    if (fileHeaderZero)
    {
        fileHeaderOffset.lock()->value = rootNode->SavedOffset();
        dump.lock()->fileHeader.Write();

        fileHeaderZero = false;
    }
}

template<typename TKey, typename TValue>
void Index<TKey, TValue>::Remove(TKey key)
{
    rootNode->Remove(key);
}

template<typename TKey, typename TValue>
IndexIterator<TKey, TValue> Index<TKey, TValue>::begin() const
{
    return IndexIterator<TKey, TValue>(rootNode->begin());
}

template<typename TKey, typename TValue>
IndexIterator<TKey, TValue> Index<TKey, TValue>::end() const
{
    return IndexIterator<TKey, TValue>(rootNode->end());
}