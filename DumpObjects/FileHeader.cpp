#include <cstring>
#include "FileHeader.h"
#include "../Dump.h"
#include "../DumpException.h"

FileHeader::FileHeader(
    Offset fileEnd, Offset pageIdIndexRoot, Offset revisionIdIndexRoot, Offset modelFormatIndexRoot,
    Offset freeSpaceIndexRoot, Offset siteInfo, weak_ptr<WritableDump> dump)
    : DumpObject(dump), FileEnd(fileEnd), PageIdIndexRoot(pageIdIndexRoot), RevisionIdIndexRoot(revisionIdIndexRoot),
    ModelFormatIndexRoot(modelFormatIndexRoot), FreeSpaceIndexRoot(freeSpaceIndexRoot), SiteInfo(siteInfo)
{}

void FileHeader::WriteInternal()
{
    stream->write("WMID", 4);
    WriteValue(FileFormatVersion);
    WriteValue(FileDataVersion);

    WriteValue(FileEnd);
    WriteValue(PageIdIndexRoot);
    WriteValue(RevisionIdIndexRoot);
    WriteValue(ModelFormatIndexRoot);
    WriteValue(FreeSpaceIndexRoot);
    WriteValue(SiteInfo);
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

    Offset fileEnd = DumpTraits<Offset>::Read(stream);
    Offset pageIdIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset revisionIdIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset modelFormatIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset freeSpaceIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset siteInfo = DumpTraits<Offset>::Read(stream);

    return FileHeader(fileEnd, pageIdIndexRoot, revisionIdIndexRoot, modelFormatIndexRoot, freeSpaceIndexRoot, siteInfo, dump.GetSelf());
}

uint32_t FileHeader::Length()
{
    return 4 + 2 * DumpTraits<uint8_t>::DumpSize() + 6 * DumpTraits<Offset>::DumpSize();
}

uint32_t FileHeader::NewLength()
{
    return Length();
}

FileHeader::FileHeader(weak_ptr<WritableDump> dump)
    : DumpObject(dump), FileEnd(Length()), PageIdIndexRoot(0), RevisionIdIndexRoot(0), ModelFormatIndexRoot(0), FreeSpaceIndexRoot(0), SiteInfo(0)
{}