#include "Offset.h"
#include "Dump.h"

Offset::Offset(int64_t value)
    : value(value)
{
    if (value < 0 || value > 0xFFFFFFFFFFFF) // 6 bytes
        throw DumpException();
}

void Offset::Write(ostream &stream) const
{
    char bytes[6];

    bytes[0] = value & 0xFF;
    bytes[1] = (value >> 8) & 0xFF;
    bytes[2] = (value >> 16) & 0xFF;
    bytes[3] = (value >> 24) & 0xFF;
    bytes[4] = (value >> 32) & 0xFF;
    bytes[5] = (value >> 40) & 0xFF;

    stream.write(bytes, 6);
}

Offset Offset::Read(istream &stream)
{
    char bytes[6];

    stream.read(bytes, 6);

    int64_t offset = 0;
    offset |= (int64_t)bytes[0];
    offset |= (int64_t)bytes[1] << 8;
    offset |= (int64_t)bytes[2] << 16;
    offset |= (int64_t)bytes[3] << 24;
    offset |= (int64_t)bytes[4] << 32;
    offset |= (int64_t)bytes[5] << 40;

    return Offset(offset);
}

int64_t Offset::DumpSize()
{
    return 6;
}