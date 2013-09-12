#include "NewRevisionChange.h"
#include "../../DumpObjects/DumpRevision.h"

NewRevisionChange NewRevisionChange::Read(std::istream &stream, bool withText)
{
    std::uint8_t modelFormatId;
    auto revision = DumpRevision::ReadCore(stream, modelFormatId, withText);
    std::uint8_t textId = 0;

    if (withText && !HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        ReadValue(stream, textId);
    }

    return NewRevisionChange(revision, modelFormatId, withText, textId);
}

void NewRevisionChange::WriteInternal()
{

    WriteValue(ChangeKind::NewRevision);
    DumpRevision::WriteCore(*stream, revision, modelFormatId, withText);

    if (withText && !HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        WriteValue(textId);
    }
}

std::uint32_t NewRevisionChange::NewLength()
{
    std::uint32_t result = 0;

    result += ValueSize(ChangeKind::NewRevision);
    result += DumpRevision::LengthCore(revision, modelFormatId, withText);

    if (withText && !HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        result += ValueSize(textId);
    }

    return result;
}