#include "Index.h"

template<typename TKey, typename TValue>
Index<TKey, TValue>::Index(weak_ptr<WritableDump> dump, weak_ptr<Offset> fileHeaderOffset)
    : dump(dump), fileHeaderOffset(fileHeaderOffset)
{
    auto offset = fileHeaderOffset.lock()->value;

    if (offset == 0)
        rootNode = IndexNode<TKey, TValue>::CreateNew(dump);
    else
        rootNode = IndexNode<TKey, TValue>::Read(dump, offset);
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

    auto offset = fileHeaderOffset.lock();

    if (rootNode->SavedOffset() != offset->value)
    {
        offset->value = rootNode->SavedOffset();
        dump.lock()->fileHeader.Write();
    }
}