#pragma once

#include <cstdint>

/** @file */

/**
 * The type of object in dump.
 */
enum class DumpObjectKind : std::uint8_t
{
    IndexLeafNode  = 0x01,
    IndexInnerNode = 0x02,

    Page           = 0x11,
    Revision       = 0x12,

    SiteInfo       = 0x21,

    TextGroup      = 0x31
};