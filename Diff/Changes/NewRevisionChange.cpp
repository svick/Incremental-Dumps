#include "NewRevisionChange.h"
#include "../../DumpObjects/DumpRevision.h"
#include "../../SevenZip.h"

void NewRevisionChange::EnsureCompressed()
{
    if (compressedTextSet)
        return;

    compressedText = SevenZip::Compress(revision.GetText());
    compressedTextSet = true;
}

NewRevisionChange NewRevisionChange::Read(std::istream &stream, bool withText)
{
    std::uint8_t modelFormatId;
    auto revision = DumpRevision::ReadCore(stream, modelFormatId, withText);

    if (withText && !HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        std::string compressedText = DumpTraits<string>::ReadLong(stream);
        revision.SetText(SevenZip::Decompress(compressedText));
    }

    return NewRevisionChange(revision, modelFormatId, withText);
}

void NewRevisionChange::WriteInternal()
{

    WriteValue(ChangeKind::NewRevision);
    DumpRevision::WriteCore(*stream, revision, modelFormatId, withText);

    if (withText && !HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        EnsureCompressed();
        DumpTraits<std::string>::WriteLong(*stream, compressedText);
    }
}

std::uint32_t NewRevisionChange::NewLength()
{
    EnsureCompressed();

    std::uint32_t result = 0;

    result += ValueSize(ChangeKind::NewRevision);
    result += DumpRevision::LengthCore(revision, modelFormatId, withText);

    if (withText && !HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        EnsureCompressed();
        result += DumpTraits<std::string>::DumpSizeLong(compressedText);
    }

    return result;
}