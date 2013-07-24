#include "Indexes/Index.h"
#include "SpaceManager.h"
#include "Dump.h"

using std::shared_ptr;

SpaceManager::SpaceManager(weak_ptr<WritableDump> dump)
    : dump(dump),
      spaceIndex(dump, shared_ptr<Offset>(dump.lock(), &dump.lock()->fileHeader.FreeSpaceIndexRoot), true),
      spaceByLength()
{
    for (auto value : spaceIndex)
    {
        spaceByLength.insert(pair<int32_t, Offset>(value.second, value.first));
    }
}

uint64_t SpaceManager::GetSpace(uint32_t length)
{
    auto foundSpace = spaceByLength.lower_bound(length);
    if (foundSpace != spaceByLength.end())
    {
        int32_t foundLength = foundSpace->first;
        Offset foundOffset = foundSpace->second;
        
        spaceByLength.erase(foundSpace);
        spaceIndex.Remove(foundOffset);

        int32_t remainingLength = foundLength - length;

        if (remainingLength != 0)
        {
            Delete(foundOffset.value + length, remainingLength);
        }

        return foundOffset.value;
    }
    else
    {
        auto dumpRef = dump.lock();
        auto &header = dumpRef->fileHeader;
        int64_t offset = header.FileEnd.value;
        header.FileEnd.value += length;
        header.Write();

        return offset;
    }
}

void SpaceManager::Delete(uint64_t offset, uint32_t length)
{
    // TODO: free space at the end just decrements fileEnd
    // TODO: join consecutive free blocks
    // TODO: zero out?

    // careful here, Add() can cause allocation of the index root node, which calls GetSpace()
    // so spaceIndex.Add() has to go before spaceByLength.insert()
    spaceIndex.Add(offset, length);
    spaceByLength.insert(pair<int32_t, Offset>(length, offset));
}
