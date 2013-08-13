#include <cstring>
#include "FileHeader.h"
#include "../Dump.h"
#include "../DumpException.h"

FileHeader::FileHeader(
    DumpKind kind,
    Offset fileEnd, Offset pageIdIndexRoot, Offset revisionIdIndexRoot, Offset modelFormatIndexRoot,
    Offset freeSpaceIndexRoot, Offset siteInfo, weak_ptr<WritableDump> dump)
    : Kind(kind), DumpObject(dump), FileEnd(fileEnd), PageIdIndexRoot(pageIdIndexRoot), RevisionIdIndexRoot(revisionIdIndexRoot),
    ModelFormatIndexRoot(modelFormatIndexRoot), FreeSpaceIndexRoot(freeSpaceIndexRoot), SiteInfo(siteInfo)
{}

void FileHeader::WriteInternal()
{
    stream->write("WMID", 4);
    WriteValue(FileFormatVersion);
    WriteValue(FileDataVersion);
    WriteValue(Kind);

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

    std::string bytes(6, '\0');
    stream.read(&bytes.at(0), 6);
    if (bytes.substr(0, 4) != "WMID" || bytes.at(4) != FileFormatVersion || bytes.at(5) != FileDataVersion)
        throw new DumpException();

    DumpKind kind = DumpTraits<DumpKind>::Read(stream);

    Offset fileEnd = DumpTraits<Offset>::Read(stream);
    Offset pageIdIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset revisionIdIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset modelFormatIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset freeSpaceIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset siteInfo = DumpTraits<Offset>::Read(stream);

    return FileHeader(kind, fileEnd, pageIdIndexRoot, revisionIdIndexRoot, modelFormatIndexRoot, freeSpaceIndexRoot, siteInfo, dump.GetSelf());
}

uint32_t FileHeader::Length()
{
    return 4 + 2 * DumpTraits<std::uint8_t>::DumpSize() + DumpTraits<DumpKind>::DumpSize() + 6 * DumpTraits<Offset>::DumpSize();
}

uint32_t FileHeader::NewLength()
{
    return Length();
}

FileHeader::FileHeader(weak_ptr<WritableDump> dump)
    : DumpObject(dump), FileEnd(Length()), PageIdIndexRoot(0), RevisionIdIndexRoot(0), ModelFormatIndexRoot(0), FreeSpaceIndexRoot(0), SiteInfo(0)
{}