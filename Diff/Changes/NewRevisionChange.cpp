#include "NewRevisionChange.h"
#include "../ChangeVisitor.h"
#include "../../DumpObjects/DumpRevision.h"

NewRevisionChange NewRevisionChange::Read(std::istream &stream, bool withText)
{
    std::uint8_t modelFormatId;
    auto revision = DumpRevision::ReadCore(stream, modelFormatId, withText);

    return NewRevisionChange(revision, modelFormatId, withText);
}

void NewRevisionChange::WriteInternal()
{
    WriteValue(ChangeKind::NewRevision);
    DumpRevision::WriteCore(*stream, revision, modelFormatId, withText);
}

std::uint32_t NewRevisionChange::NewLength()
{
    return ValueSize(ChangeKind::NewRevision)
        + DumpRevision::LengthCore(revision, modelFormatId, withText, revision.GetCompressedText().length());
}

void NewRevisionChange::Accept(ChangeVisitor &visitor)
{
    visitor.Visit(*this);
}