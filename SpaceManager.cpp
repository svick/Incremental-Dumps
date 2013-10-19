#include "Indexes/Index.h"
#include "SpaceManager.h"
#include "Dump.h"

SpaceManager::SpaceManager(std::weak_ptr<Dump> dump)
    : dump(dump),
      spaceByLength()
{
    for (auto value : *dump.lock()->spaceIndex)
    {
        spaceByLength.insert(std::pair<std::int32_t, Offset>(value.second, value.first));
    }
}

std::uint64_t SpaceManager::GetSpace(std::uint32_t length)
{
    auto foundSpace = spaceByLength.lower_bound(length);
    if (foundSpace != spaceByLength.end())
    {
        std::int32_t foundLength = foundSpace->first;
        Offset foundOffset = foundSpace->second;
        
        spaceByLength.erase(foundSpace);
        dump.lock()->spaceIndex->Remove(foundOffset);

        std::int32_t remainingLength = foundLength - length;

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
        std::int64_t offset = header.FileEnd.value;
        header.FileEnd.value += length;
        header.Write();

        return offset;
    }
}

void SpaceManager::Delete(std::uint64_t offset, std::uint32_t length)
{
    // TODO: free space at the end just decrements fileEnd
    // TODO: join consecutive free blocks

    // careful here, Add() can cause allocation of the index root node, which calls GetSpace()
    // so spaceIndex.Add() has to go before spaceByLength.insert()
    dump.lock()->spaceIndex->Add(offset, length);
    spaceByLength.insert(std::make_pair(length, offset));
}
