#include "Offset.h"
#include "../DumpException.h"

Offset::Offset(uint64_t value)
    : value(value)
{
    if (value < 0 || value > 0xFFFFFFFFFFFF) // 6 bytes
        throw DumpException();
}

bool operator <(const Offset &first, const Offset &second)
{
    return first.value < second.value;
}