#include "Index.h"
#include "../SpaceManager.h"

#include <memory>

using std::move;

template<typename TKey, typename TValue>
Index<TKey, TValue>::Index(std::weak_ptr<WritableDump> dump, Offset *fileHeaderOffset, bool delaySave)
    : dump(dump), fileHeaderOffset(std::shared_ptr<Offset>(dump.lock(), fileHeaderOffset)), recentAccesses(0)
{
    rootNodeUnsaved = false;

    if (fileHeaderOffset->value == 0)
    {
        rootNode = IndexNode<TKey, TValue>::CreateNew(dump);

        // delaySave is not the only option, so that root nodes of most indexes were at the start of the file
        if (delaySave)
        {
            rootNodeUnsaved = true;
        }
        else
        {
            rootNode->Write(true);
            fileHeaderOffset->value = rootNode->SavedOffset();
            dump.lock()->fileHeader.Write();
        }
    }
    else
        rootNode = IndexNode<TKey, TValue>::Read(dump, fileHeaderOffset->value);
}

template<typename TKey, typename TValue>
TValue Index<TKey, TValue>::Get(TKey key)
{
    auto result = rootNode->Get(key);

    AfterAccess();

    return result;
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
        rootNode->Write(true);

        fileHeaderOffset.lock()->value = rootNode->SavedOffset();
        dump.lock()->fileHeader.Write();

        rootNodeUnsaved = false;
    }

    AfterAccess();
}

template<typename TKey, typename TValue>
void Index<TKey, TValue>::AfterAccess()
{
    recentAccesses++;

    if (recentAccesses >= 10000)
    {
        if (rootNode->NodesCount() >= 5000)
            rootNode->ClearCached();

        recentAccesses = 0;
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