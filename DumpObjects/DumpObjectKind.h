#pragma once

#include <cstdint>

using std::uint8_t;

enum class DumpObjectKind : uint8_t
{
    IndexLeafNode  = 0x01,
    IndexInnerNode = 0x02,

    Page           = 0x11,
    Revision       = 0x12
};