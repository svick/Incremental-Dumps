#include "SpaceManager.h"
#include "Dump.h"

SpaceManager::SpaceManager(weak_ptr<WritableDump> dump)
    : dump(dump)
{}

int64_t SpaceManager::GetSpace(int64_t length)
{
    // TODO: find free space

    auto dumpRef = dump.lock();
    auto &header = dumpRef->fileHeader;
    int64_t offset = header.FileEnd.value;
    header.FileEnd.value += length;
    header.Write();

    return offset;
}

void SpaceManager::Delete(int64_t offset)
{
    // TODO
    throw new DumpException();
}