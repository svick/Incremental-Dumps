#include "Index.h"
#include "../SpaceManager.h"

#include <memory>

using std::move;

template<typename TKey, typename TValue>
Index<TKey, TValue>::Index(std::weak_ptr<WritableDump> dump, std::weak_ptr<Offset> fileHeaderOffset, bool delaySave)
    : dump(dump), fileHeaderOffset(fileHeaderOffset), recentChanges(0)
{
    auto offset = fileHeaderOffset.lock();

    rootNodeUnsaved = false;

    if (offset->value == 0)
    {
        rootNode = IndexNode<TKey, TValue>::CreateNew(dump);

        if (delaySave)
        {
            rootNodeUnsaved = true;
        }
        else
        {
            rootNode->Write();
            fileHeaderOffset.lock()->value = rootNode->SavedOffset();
            dump.lock()->fileHeader.Write();
        }
    }
    else
        rootNode = IndexNode<TKey, TValue>::Read(dump, offset->value);
}

template<typename TKey, typename TValue>
TValue Index<TKey, TValue>::Get(TKey key)
{
    return rootNode->Get(key);
}

template<typename TKey, typename TValue>
void Index<TKey, TValue>::AfterAdd()
{
    if (rootNode->IsOversized())
    {
        dump.lock()->spaceManager->Delete(rootNode->SavedOffset(), rootNode->NewLength());

        auto splitted = rootNode->Split();
        splitted.LeftNode->Write();
        splitted.RightNode->Write();
        rootNode = std::unique_ptr<IndexNode<TKey, TValue>>(
            new IndexInnerNode<TKey, TValue>(dump, std::move(splitted)));

        rootNodeUnsaved = true;
    }

    if (rootNodeUnsaved)
    {
        rootNode->Write();

        fileHeaderOffset.lock()->value = rootNode->SavedOffset();
        dump.lock()->fileHeader.Write();

        rootNodeUnsaved = false;
    }

    recentChanges++;

    if (recentChanges >= 100000)
    {
        rootNode->ClearCached();
        recentChanges = 0;
    }
}

template<typename TKey, typename TValue>
void Index<TKey, TValue>::Add(TKey key, TValue value)
{
    rootNode->Add(key, value);

    AfterAdd();
}

template<typename TKey, typename TValue>
void Index<TKey, TValue>::AddOrUpdate(TKey key, TValue value)
{
    rootNode->AddOrUpdate(key, value);

    AfterAdd();
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

template<typename TKey, typename TValue>
void Index<TKey, TValue>::Write()
{
    rootNode->Write();
}