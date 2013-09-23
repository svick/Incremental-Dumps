#include "NewPageChange.h"
#include "../../DumpObjects/DumpPage.h"

NewPageChange NewPageChange::Read(std::istream &stream)
{
    return NewPageChange(DumpPage::ReadCore(stream));
}

void NewPageChange::WriteInternal()
{
    WriteValue(ChangeKind::NewPage);
    DumpPage::WriteCore(*stream, page);
}

std::uint32_t NewPageChange::NewLength()
{
    return ValueSize(ChangeKind::NewPage) + DumpPage::LengthCore(page);
}