#include "DiffTextGroup.h"

DiffTextGroup::DiffTextGroup(const std::string &compressedTexts)
    : compressedTexts(compressedTexts)
{}

DiffTextGroup DiffTextGroup::Read(std::istream &stream)
{
    auto texts = DumpTraits<std::string>::ReadLong(stream);

    return DiffTextGroup(texts);
}

std::uint32_t DiffTextGroup::NewLength()
{
    return ValueSize(ChangeKind::TextGroup)
        + DumpTraits<std::string>::DumpSizeLong(compressedTexts);
}

void DiffTextGroup::WriteInternal()
{
    WriteValue(ChangeKind::TextGroup);
    DumpTraits<std::string>::WriteLong(*stream, compressedTexts);
}