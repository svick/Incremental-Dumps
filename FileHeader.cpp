#include "FileHeader.h"
#include "Dump.h"

FileHeader::FileHeader(Offset fileEnd, Offset pageIdIndexRoot, Offset freeSpaceIndexRoot)
    : FileEnd(fileEnd), PageIdIndexRoot(pageIdIndexRoot), FreeSpaceIndexRoot(freeSpaceIndexRoot)
{
}

void FileHeader::WriteInternal(unique_ptr<iostream> const &stream)
{
    stream->write("WMID", 4);
    stream->write(&FileFormatVersion, 1);
    stream->write(&FileDataVersion, 1);

    FileEnd.Write(stream);
    PageIdIndexRoot.Write(stream);
    FreeSpaceIndexRoot.Write(stream);
}

FileHeader FileHeader::Read(unique_ptr<iostream> const &stream)
{
    char bytes[6];
    stream->read(bytes, 6);
    if (stream->fail() || strncmp(bytes, "WMID", 4) != 0 || bytes[4] != FileFormatVersion || bytes[5] != FileDataVersion)
        throw new DumpException();

    Offset fileEnd = Offset::Read(stream);
    Offset pageIdIndexRoot = Offset::Read(stream);
    Offset freeSpaceIndexRoot = Offset::Read(stream);

    return FileHeader(fileEnd, pageIdIndexRoot, freeSpaceIndexRoot);
}

FileHeader::FileHeader()
    : FileEnd(0), PageIdIndexRoot(0), FreeSpaceIndexRoot(0)
{
}