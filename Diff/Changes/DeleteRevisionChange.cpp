#include "DeleteRevisionChange.h"

DeleteRevisionChange DeleteRevisionChange::Read(std::istream &stream)
{
    std::uint32_t revisionId;
    ReadValue(stream, revisionId);

    return DeleteRevisionChange(revisionId);
}

void DeleteRevisionChange::WriteInternal()
{
    WriteValue(ChangeKind::DeleteRevision);
    WriteValue(revisionId);
}

std::uint32_t DeleteRevisionChange::NewLength()
{
    return ValueSize(ChangeKind::DeleteRevision) + ValueSize(revisionId);
}