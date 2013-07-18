#include <cstring>
#include "FileHeader.h"
#include "../Dump.h"
#include "../DumpException.h"

FileHeader::FileHeader(Offset fileEnd, Offset pageIdIndexRoot, Offset revisionIdIndexRoot, Offset freeSpaceIndexRoot, weak_ptr<WritableDump> dump)
    : DumpObject(dump), FileEnd(fileEnd), PageIdIndexRoot(pageIdIndexRoot), RevisionIdIndexRoot(revisionIdIndexRoot), FreeSpaceIndexRoot(freeSpaceIndexRoot)
{}

void FileHeader::WriteInternal()
{
    stream->write("WMID", 4);
    DumpTraits<uint8_t>::Write(*stream, FileFormatVersion);
    DumpTraits<uint8_t>::Write(*stream, FileDataVersion);

    FileEnd.Write(*stream);
    PageIdIndexRoot.Write(*stream);
    RevisionIdIndexRoot.Write(*stream);
    FreeSpaceIndexRoot.Write(*stream);
}

void FileHeader::Write()
{
    auto dumpRef = dump.lock();
    stream = dumpRef->stream.get();

    stream->seekp(0);

    WriteInternal();

    stream = nullptr;
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
    Offset revisionIdIndexRoot = Offset::Read(stream);
    Offset freeSpaceIndexRoot = Offset::Read(stream);

    return FileHeader(fileEnd, pageIdIndexRoot, revisionIdIndexRoot, freeSpaceIndexRoot, dump.GetSelf());
}

uint32_t FileHeader::Length()
{
    return 4 + 2 * DumpTraits<uint8_t>::DumpSize() + 4 * DumpTraits<Offset>::DumpSize();
}

uint32_t FileHeader::NewLength() const
{
    return Length();
}

FileHeader::FileHeader(weak_ptr<WritableDump> dump)
    : DumpObject(dump), FileEnd(Length()), PageIdIndexRoot(0), FreeSpaceIndexRoot(0)
{}
