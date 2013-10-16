#pragma once

#include <cstdint>

/**
 * Represents an offset into the dump file.
 *
 * Is saved as 6-byte integer on disk,
 */
class Offset
{
public:
    uint64_t value;

    Offset(uint64_t value = 0);
};

bool operator <(const Offset &first, const Offset &second);