#include "StringHelpers.h"
#include <sstream>

// http://stackoverflow.com/a/236803/41071
std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

long tryParseLong(const std::string &s, bool &success, int radix)
{
    char* end;
    const char* start = s.c_str();
    long result = strtol(start, &end, radix);

    if (errno == 0 && end != start && *end=='\0')
    {
        success = true;
        return result;
    }
    else
    {
        success = false;
        return 0;
    }
}