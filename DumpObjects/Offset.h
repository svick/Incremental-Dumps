#pragma once

#include <cstdint>
#include <iostream>

using std::int64_t;
using std::istream;
using std::ostream;

class Offset
{
public:
    int64_t value;

    Offset(int64_t value);
    void Write(ostream &stream) const;
    static Offset Read(istream &stream);
    static int32_t DumpSize();
};

bool operator <(const Offset &first, const Offset &second);