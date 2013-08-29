#include "PartialDeletePageChange.h"
#include "../ChangeVisitor.h"

PartialDeletePageChange PartialDeletePageChange::Read(std::istream &stream)
{
    std::uint32_t pageId;
    ReadValue(stream, pageId);

    return PartialDeletePageChange(pageId);
}

void PartialDeletePageChange::WriteInternal()
{
    WriteValue(ChangeKind::DeletePagePartial);
    WriteValue(pageId);
}

std::uint32_t PartialDeletePageChange::NewLength()
{
    return ValueSize(ChangeKind::DeletePagePartial) + ValueSize(pageId);
}

void PartialDeletePageChange::Accept(ChangeVisitor &visitor)
{
    visitor.Visit(*this);
}