#include "FileHeader.h"
#include "../Dump.h"
#include "../DumpException.h"

FileHeader::FileHeader(Offset fileEnd, Offset pageIdIndexRoot, Offset freeSpaceIndexRoot, weak_ptr<WritableDump> dump)
    : DumpObject(dump), FileEnd(fileEnd), PageIdIndexRoot(pageIdIndexRoot), FreeSpaceIndexRoot(freeSpaceIndexRoot)
{}

void FileHeader::Write(ostream &stream)
{
    stream.write("WMID", 4);
    stream.write(&FileFormatVersion, 1);
    stream.write(&FileDataVersion, 1);

    FileEnd.Write(stream);
    PageIdIndexRoot.Write(stream);
    FreeSpaceIndexRoot.Write(stream);
}

void FileHeader::Write()
{
    auto dumpRef = dump.lock();
    ostream &stream = *(dumpRef->stream);

    stream.seekp(0);
    Write(stream);
}

FileHeader FileHeader::Read(ReadableDump const &dump)
{
    istream &stream = *(dump.stream);

    char bytes[6];
    stream.read(bytes, 6);
    if (strncmp(bytes, "WMID", 4) != 0 || bytes[4] != FileFormatVersion || bytes[5] != FileDataVersion)
        throw new DumpException();

    Offset fileEnd = Offset::Read(stream);
    Offset pageIdIndexRoot = Offset::Read(stream);
    Offset freeSpaceIndexRoot = Offset::Read(stream);

    return FileHeader(fileEnd, pageIdIndexRoot, freeSpaceIndexRoot, dump.GetSelf());
}

int32_t FileHeader::NewLength()
{
    return 6 + 3 * 6;
}

FileHeader::FileHeader(weak_ptr<WritableDump> dump)
    : DumpObject(dump), FileEnd(6 + 3 * 6), PageIdIndexRoot(0), FreeSpaceIndexRoot(0)
{}