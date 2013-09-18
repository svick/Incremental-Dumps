#include "StringHelpers.h"
#include "DumpException.h"
#include "format.h"
#include <sstream>

// http://stackoverflow.com/a/236803/41071
std::vector<std::string> split(std::string s, char delim)
{
    // getline() treats delim as terminator, not separator, this fixes that
    if (s.back() == delim)
        s.push_back(delim);

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
    if (errno != 0)
    {
        throw DumpException(
            str(fmt::Format("errno was already set ({0}: {1})") << errno << strerror(errno)));
    }

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
        errno = 0;
        success = false;
        return 0;
    }
}
