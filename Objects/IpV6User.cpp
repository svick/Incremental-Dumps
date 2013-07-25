#include "IpV6User.h"
#include <sstream>
#include "../StringHelpers.h"
#include "../DumpException.h"
#include "Revision.h"

using std::stoi;
using std::ostringstream;

std::array<uint16_t, 8> IpV6User::TryParseAddress(string address, bool &success)
{
    success = false;

    std::vector<std::string> stringParts = split(address, ':');
    if (stringParts.size() != 8)
        return std::array<uint16_t, 8>();

    std::array<uint16_t, 8> result;

    for (int i = 0; i < 8; i++)
    {
        bool success;
        long intPart = tryParseLong(stringParts[i], success, 16);
        if (!success || intPart > 0xFFFF || intPart < 0)
            return std::array<uint16_t, 8>();
        
        result[i] = intPart;
    }

    success = true;
    return result;
}

std::array<uint16_t, 8> IpV6User::ParseAddress(string address)
{
    bool success;
    auto result = TryParseAddress(address, success);

    if (!success)
        throw DumpException();

    return result;
}

string IpV6User::AddressToString(std::array<uint16_t, 8> address)
{
    ostringstream s;

    s << std::hex << std::uppercase;

    for (int i = 0; i < 8; i++)
    {
        if (i != 0)
            s << ':';

        s << address[i];
    }

    return s.str();
}

IpV6User::IpV6User(string stringAddress)
    : User(0, stringAddress), Address(ParseAddress(stringAddress))
{
}

IpV6User::IpV6User(string stringAddress, std::array<uint16_t, 8> parsedAddress)
    :  User(0, stringAddress), Address(parsedAddress)
{}

IpV6User::IpV6User(std::array<uint16_t, 8> parsedAddress)
    : User(0, AddressToString(parsedAddress)), Address(parsedAddress)
{}

RevisionFlags IpV6User::UserKind() const
{
    return RevisionFlags::IpV6User;
}