#include "DumpObject.h"
#include "../Dump.h"

DumpObject::DumpObject(weak_ptr<WritableDump> dump)
    : dump(dump), savedOffset(0), savedLength(0)
{}

void DumpObject::Write()
{
    auto dumpRef = dump.lock();

    int32_t newLength = NewLength();
    int64_t newOffset;

    if (newLength == savedLength)
        newOffset = savedOffset;
    else
    {
        auto &spaceManager = dumpRef->spaceManager;

        if (savedOffset != 0)
            spaceManager->Delete(savedOffset, savedLength);

        newOffset = spaceManager->GetSpace(newLength);
    }

    ostream& stream = *(dumpRef->stream);
    stream.seekp(newOffset);

    Write(stream);

    savedOffset = newOffset;
    savedLength = newLength;
}

int64_t DumpObject::SavedOffset()
{
    return savedOffset;
}