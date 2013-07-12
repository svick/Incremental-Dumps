#include "Timestamp.h"

#include <sstream>
#include <cstdio>

using std::istringstream;

Timestamp::Timestamp()
    : Year(2000), Month(1), Day(1), Hour(0), Minute(0), Second(0)
{}

Timestamp::Timestamp(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
    : Year(year), Month(month), Day(day), Hour(hour), Minute(minute), Second(second)
{}

Timestamp::Timestamp(uint32_t integer)
{
    Second = integer % 60;
    integer /= 60;
    Minute = integer % 60;
    integer /= 60;
    Hour = integer % 24;
    integer /= 24;
    Day = integer % 31 + 1;
    integer /= 31;
    Month = integer % 12 + 1;
    integer /= 12;
    Year = integer + 2000;
}

Timestamp::Timestamp(string s)
{
    istringstream stream(s);

    unsigned dateParts[6];

    for (int i = 0; i < 6; i++)
    {
        char delim;
        stream >> dateParts[i] >> delim;
    }

    Year = dateParts[0];
    Month = dateParts[1];
    Day = dateParts[2];
    Hour = dateParts[3];
    Minute = dateParts[4];
    Second = dateParts[5];
}

uint32_t Timestamp::ToInteger() const
{
    uint32_t result = Year - 2000;
    result = result * 12 + Month - 1;
    result = result * 31 + Day - 1;
    result = result * 24 + Hour;
    result = result * 60 + Minute;
    result = result * 60 + Second;

    return result;
}

string Timestamp::ToString() const
{
    char chars[25];
    int count = sprintf(chars, "%u-%02u-%02uT%02u:%02u:%02uZ", Year, (unsigned)Month, (unsigned)Day, (unsigned)Hour, (unsigned)Minute, (unsigned)Second);

    return string(chars, count);
}