#include "IpV4User.h"
#include <sstream>
#include "../StringHelpers.h"
#include "../DumpException.h"

using std::stoi;
using std::ostringstream;

uint32_t IpV4User::ParseAddress(string address)
{
    vector<string> stringParts = split(address, '.');
    if (stringParts.size() != 4)
        return 0;

    uint32_t result = 0;

    for (int i = 0; i < 4; i++)
    {
        int intPart = stoi(stringParts[i]);
        if (intPart > 255 || intPart < 0)
            return 0;
        
        result |= (uint32_t)(uint8_t)intPart << (8 * i);
    }

    return result;
}

string IpV4User::AddressToString(uint32_t address)
{
    ostringstream s;

    for (int i = 0; i < 4; i++)
    {
        if (i != 0)
            s << '.';

        unsigned part = (address >> (8 * i)) & 0xFF;
        s << part;
    }

    return s.str();
}

IpV4User::IpV4User(string stringAddress)
    : User(0, stringAddress), Address(ParseAddress(stringAddress))
{
    if (Address == 0)
        throw new DumpException();
}

IpV4User::IpV4User(string stringAddress, uint32_t parsedAddress)
    :  User(0, stringAddress), Address(parsedAddress)
{
    if (Address == 0)
        throw new DumpException();
}

IpV4User::IpV4User(uint32_t parsedAddress)
    : User(0, AddressToString(parsedAddress)), Address(parsedAddress)
{}