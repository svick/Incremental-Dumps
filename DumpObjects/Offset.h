#pragma once

#include <cstdint>
#include <iostream>

using std::uint64_t;
using std::istream;
using std::ostream;

class Offset
{
public:
    uint64_t value;

    Offset(uint64_t value = 0);
};

bool operator <(const Offset &first, const Offset &second);