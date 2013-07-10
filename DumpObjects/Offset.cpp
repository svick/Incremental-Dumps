#include "Offset.h"
#include "../DumpException.h"

Offset::Offset(uint64_t value)
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

    uint64_t offset = 0;
    offset |= (uint64_t)(uint8_t)bytes[0];
    offset |= (uint64_t)(uint8_t)bytes[1] << 8;
    offset |= (uint64_t)(uint8_t)bytes[2] << 16;
    offset |= (uint64_t)(uint8_t)bytes[3] << 24;
    offset |= (uint64_t)(uint8_t)bytes[4] << 32;
    offset |= (uint64_t)(uint8_t)bytes[5] << 40;

    return Offset(offset);
}

uint32_t Offset::DumpSize()
{
    return 6;
}

bool operator <(const Offset &first, const Offset &second)
{
    return first.value < second.value;
}