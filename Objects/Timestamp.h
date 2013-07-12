#pragma once

#include <cstdint>
#include <string>

using std::uint32_t;
using std::uint16_t;
using std::uint8_t;
using std::string;

// represents a timestamp between 2000 and about 2100, with second accuracy
class Timestamp
{
public:
    uint16_t Year;
    uint8_t Month;
    uint8_t Day;
    uint8_t Hour;
    uint8_t Minute;
    uint8_t Second;

    Timestamp();
    Timestamp(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second);
    Timestamp(uint32_t integer);
    Timestamp(string s);

    // converts timestamp to a 32-bit unsigned integer
    // the value does *not* represent the number of seconds since some date, or something like that
    uint32_t ToInteger() const;
    string ToString() const;
};