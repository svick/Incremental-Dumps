#include "FileHeader.h"
#include "../Dump.h"
#include "../DumpException.h"

FileHeader::FileHeader(Offset fileEnd, Offset pageIdIndexRoot, Offset freeSpaceIndexRoot, weak_ptr<WritableDump> dump)
    : DumpObject(dump), FileEnd(fileEnd), PageIdIndexRoot(pageIdIndexRoot), FreeSpaceIndexRoot(freeSpaceIndexRoot)
{}

void FileHeader::WriteInternal()
{
    stream->write("WMID", 4);
    DumpTraits<uint8_t>::Write(*stream, FileFormatVersion);
    DumpTraits<uint8_t>::Write(*stream, FileDataVersion);

    FileEnd.Write(*stream);
    PageIdIndexRoot.Write(*stream);
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
    Offset freeSpaceIndexRoot = Offset::Read(stream);

    return FileHeader(fileEnd, pageIdIndexRoot, freeSpaceIndexRoot, dump.GetSelf());
}

uint32_t FileHeader::NewLength() const
{
    return 4 + 2 * DumpTraits<uint8_t>::DumpSize() + 3 * DumpTraits<Offset>::DumpSize();
}

FileHeader::FileHeader(weak_ptr<WritableDump> dump)
    : DumpObject(dump), FileEnd(6 + 3 * 6), PageIdIndexRoot(0), FreeSpaceIndexRoot(0)
{}