#include "DumpObject.h"
#include "../Dump.h"

DumpObject::DumpObject(weak_ptr<WritableDump> dump)
    : dump(dump), savedOffset(0), savedLength(0)
{}

void DumpObject::Write()
{
    auto dumpRef = dump.lock();

    uint32_t newLength = NewLength();
    uint64_t newOffset;

    if (newLength == savedLength)
        newOffset = savedOffset;
    else
    {
        auto &spaceManager = dumpRef->spaceManager;

        if (savedOffset != 0)
            spaceManager->Delete(savedOffset, savedLength);

        newOffset = spaceManager->GetSpace(newLength);
    }

    stream = dumpRef->stream.get();
    stream->seekp(newOffset);

    WriteInternal();

    stream = nullptr;

    bool overwriteIndex = savedOffset != 0;

    savedOffset = newOffset;
    savedLength = newLength;

    UpdateIndex(newOffset, overwriteIndex);
}

void DumpObject::UpdateIndex(Offset offset, bool overwrite)
{}

uint64_t DumpObject::SavedOffset() const
{
    return savedOffset;
}