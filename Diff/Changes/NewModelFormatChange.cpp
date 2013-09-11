#include "NewModelFormatChange.h"

NewModelFormatChange NewModelFormatChange::Read(std::istream &stream)
{
    std::uint8_t id;
    std::string model, format;

    ReadValue(stream, id);
    ReadValue(stream, model);
    ReadValue(stream, format);

    return NewModelFormatChange(id, model, format);
}

void NewModelFormatChange::WriteInternal()
{
    WriteValue(ChangeKind::NewModelFormat);
    WriteValue(id);
    WriteValue(model);
    WriteValue(format);
}

std::uint32_t NewModelFormatChange::NewLength()
{
    return ValueSize(ChangeKind::NewModelFormat) + ValueSize(id)
        + ValueSize(model) + ValueSize(format);
}