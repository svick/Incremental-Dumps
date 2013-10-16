#include "IpV4User.h"
#include <sstream>
#include "../StringHelpers.h"
#include "../DumpException.h"
#include "Revision.h"

std::uint32_t IpV4User::TryParseAddress(std::string address, bool &success)
{
    success = false;

    std::vector<std::string> stringParts = split(address, '.');
    if (stringParts.size() != 4)
        return 0;

    uint32_t result = 0;

    for (int i = 0; i < 4; i++)
    {
        bool parseSuccess;
        long intPart = tryParseLong(stringParts.at(i), parseSuccess);

        if (!parseSuccess || intPart > 255 || intPart < 0)
            return 0;
        
        result |= (uint32_t)(uint8_t)intPart << (8 * i);
    }

    success = true;
    return result;
}

std::uint32_t IpV4User::ParseAddress(std::string address)
{
    bool success;
    std::uint32_t result = TryParseAddress(address, success);

    if (!success)
        throw DumpException();

    return result;
}

std::string IpV4User::AddressToString(std::uint32_t address)
{
    std::ostringstream s;

    for (int i = 0; i < 4; i++)
    {
        if (i != 0)
            s << '.';

        unsigned part = (address >> (8 * i)) & 0xFF;
        s << part;
    }

    return s.str();
}

IpV4User::IpV4User(std::string stringAddress)
    : User(0, stringAddress), Address(ParseAddress(stringAddress))
{}

IpV4User::IpV4User(std::string stringAddress, std::uint32_t parsedAddress)
    :  User(0, stringAddress), Address(parsedAddress)
{}

IpV4User::IpV4User(std::uint32_t parsedAddress)
    : User(0, AddressToString(parsedAddress)), Address(parsedAddress)
{}

bool IpV4User::Equals(const User &second) const
{
    auto otherCasted = dynamic_cast<const IpV4User*>(&second);
    if (otherCasted == nullptr)
        return false;

    return this->Address == otherCasted->Address;
}

RevisionFlags IpV4User::UserKind() const
{
    return RevisionFlags::IpV4User;
}
