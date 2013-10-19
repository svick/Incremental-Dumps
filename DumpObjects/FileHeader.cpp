#include <cstring>
#include "FileHeader.h"
#include "../Dump.h"
#include "../DumpException.h"

const std::string FileHeader::MagicNumber = "MWID";
const std::uint8_t FileHeader::FileFormatVersion = 1;
const std::uint8_t FileHeader::FileDataVersion = 1;

FileHeader::FileHeader(
    DumpKind kind,
    Offset fileEnd, Offset pageIdIndexRoot, Offset revisionIdIndexRoot,
    Offset textGroupIdIndexRoot, Offset modelFormatIndexRoot,
    Offset freeSpaceIndexRoot, Offset siteInfo, std::weak_ptr<Dump> dump)
    : DumpObject(dump), Kind(kind), FileEnd(fileEnd), PageIdIndexRoot(pageIdIndexRoot), RevisionIdIndexRoot(revisionIdIndexRoot),
        TextGroupIdIndexRoot(textGroupIdIndexRoot), ModelFormatIndexRoot(modelFormatIndexRoot),
        FreeSpaceIndexRoot(freeSpaceIndexRoot), SiteInfo(siteInfo)
{}

void FileHeader::WriteInternal()
{
    stream->write(MagicNumber.data(), MagicNumber.length());
    WriteValue(FileFormatVersion);
    WriteValue(FileDataVersion);
    WriteValue(Kind);

    WriteValue(FileEnd);
    WriteValue(PageIdIndexRoot);
    WriteValue(RevisionIdIndexRoot);
    WriteValue(TextGroupIdIndexRoot);
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

FileHeader FileHeader::Read(Dump const &dump)
{
    std::istream &stream = *(dump.stream);

    std::string magicNumber(MagicNumber.length(), '\0');
    stream.read(&magicNumber.at(0), MagicNumber.length());

    std::uint8_t fileFormatVersion, fileDataVersion;
    ReadValue(stream, fileFormatVersion);
    ReadValue(stream, fileDataVersion);

    if (magicNumber != MagicNumber
        || fileFormatVersion != FileFormatVersion
        || fileDataVersion != FileDataVersion)
        throw new DumpException();

    DumpKind kind = DumpTraits<DumpKind>::Read(stream);

    Offset fileEnd = DumpTraits<Offset>::Read(stream);
    Offset pageIdIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset revisionIdIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset textGroupIdIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset modelFormatIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset freeSpaceIndexRoot = DumpTraits<Offset>::Read(stream);
    Offset siteInfo = DumpTraits<Offset>::Read(stream);

    return FileHeader(kind, fileEnd, pageIdIndexRoot, revisionIdIndexRoot, textGroupIdIndexRoot, modelFormatIndexRoot, freeSpaceIndexRoot, siteInfo, dump.GetSelf());
}

uint32_t FileHeader::Length()
{
    return 4 + 2 * DumpTraits<std::uint8_t>::DumpSize() + DumpTraits<DumpKind>::DumpSize() + 7 * DumpTraits<Offset>::DumpSize();
}

uint32_t FileHeader::NewLength()
{
    return Length();
}

FileHeader::FileHeader(std::weak_ptr<Dump> dump)
    : DumpObject(dump), FileEnd(Length()), PageIdIndexRoot(0), RevisionIdIndexRoot(0), TextGroupIdIndexRoot(0), ModelFormatIndexRoot(0), FreeSpaceIndexRoot(0), SiteInfo(0)
{}
