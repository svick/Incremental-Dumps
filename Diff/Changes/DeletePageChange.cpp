#include "DeletePageChange.h"
#include "../ChangeVisitor.h"

DeletePageChange DeletePageChange::Read(std::istream &stream)
{
    std::uint32_t pageId;
    ReadValue(stream, pageId);

    return DeletePageChange(pageId);
}

void DeletePageChange::WriteInternal()
{
    WriteValue(ChangeKind::DeletePage);
    WriteValue(pageId);
}

std::uint32_t DeletePageChange::NewLength()
{
    return ValueSize(ChangeKind::DeletePage) + ValueSize(pageId);
}

void DeletePageChange::Accept(ChangeVisitor &visitor)
{
    visitor.Visit(*this);
}