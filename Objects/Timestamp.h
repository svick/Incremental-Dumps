#pragma once

#include <cstdint>
#include <string>

using std::uint32_t;
using std::uint16_t;
using std::uint8_t;
using std::string;

/**
* Represents a timestamp between the year 2000 and more than 2100, with accuracy down to 1 second.
*/
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
    /**
     * @param s A UTC timestamp string in ISO 8601 format, for example <tt>2013-09-21T10:02Z</tt>.
     */
    Timestamp(string s);

    /**
     * Converts the timestamp to a 32-bit unsigned integer.
     *
     * The value does *not* represent the number of seconds since some date, or something like that.
     */
    uint32_t ToInteger() const;
    /**
     * Converts the timestamp to ISO 8601 format.
     */
    string ToString() const;
};

bool operator ==(const Timestamp &first, const Timestamp &second);
bool operator !=(const Timestamp &first, const Timestamp &second);