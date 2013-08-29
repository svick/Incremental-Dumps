#include "FullDeletePageChange.h"
#include "../ChangeVisitor.h"

FullDeletePageChange FullDeletePageChange::Read(std::istream &stream)
{
    std::uint32_t pageId;
    ReadValue(stream, pageId);

    return FullDeletePageChange(pageId);
}

void FullDeletePageChange::WriteInternal()
{
    WriteValue(ChangeKind::DeletePageFull);
    WriteValue(pageId);
}

std::uint32_t FullDeletePageChange::NewLength()
{
    return ValueSize(ChangeKind::DeletePageFull) + ValueSize(pageId);
}

void FullDeletePageChange::Accept(ChangeVisitor &visitor)
{
    visitor.Visit(*this);
}