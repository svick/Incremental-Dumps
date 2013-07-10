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
    void Write(ostream &stream) const;
    static Offset Read(istream &stream);
    static uint32_t DumpSize();
};

bool operator <(const Offset &first, const Offset &second);