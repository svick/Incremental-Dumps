#include "IpV6User.h"
#include <sstream>
#include "../StringHelpers.h"
#include "../CollectionHelpers.h"
#include "../DumpException.h"
#include "Revision.h"

bool expandEmptyGroup(std::vector<std::string>& groups)
{
    if (groups.size() < 3)
        return false;

    if (groups.at(0).empty())
    {
        if (groups.at(1).empty())
            groups.erase(groups.begin());
        else
            return false;
    }

    if (groups.rbegin()->empty())
    {
        if ((groups.rbegin() + 1)->empty())
            groups.pop_back();
        else
            return false;
    }

    auto emptyGroupIter = find(groups, std::string());

    if (emptyGroupIter != groups.end())
    {
        auto insertIter = groups.erase(emptyGroupIter);

        groups.insert(insertIter, 8 - groups.size(), "0");
    }

    return true;
}

std::array<std::uint16_t, 8> IpV6User::TryParseAddress(const std::string& address, bool &success)
{
    std::vector<std::string> groups = split(address, ':');

    success = expandEmptyGroup(groups);
    if (!success || groups.size() != 8)
        return std::array<std::uint16_t, 8>();

    std::array<std::uint16_t, 8> result;

    for (int i = 0; i < 8; i++)
    {
        bool success;
        long intGroup = tryParseLong(groups.at(i), success, 16);
        if (!success || intGroup > 0xFFFF || intGroup < 0)
            return std::array<std::uint16_t, 8>();
        
        result.at(i) = intGroup;
    }

    success = true;
    return result;
}

std::array<std::uint16_t, 8> IpV6User::ParseAddress(const std::string& address)
{
    bool success;
    auto result = TryParseAddress(address, success);

    if (!success)
        throw DumpException();

    return result;
}

std::string IpV6User::AddressToString(std::array<std::uint16_t, 8> address)
{
    std::ostringstream s;

    s << std::hex << std::uppercase;

    for (int i = 0; i < 8; i++)
    {
        if (i != 0)
            s << ':';

        s << address.at(i);
    }

    return s.str();
}

IpV6User::IpV6User(const std::string& stringAddress)
    : User(0, stringAddress), Address(ParseAddress(stringAddress))
{
}

IpV6User::IpV6User(const std::string& stringAddress, std::array<uint16_t, 8> parsedAddress)
    :  User(0, stringAddress), Address(parsedAddress)
{}

IpV6User::IpV6User(std::array<uint16_t, 8> parsedAddress)
    : User(0, AddressToString(parsedAddress)), Address(parsedAddress)
{}

bool IpV6User::Equals(const User &second) const
{
    auto otherCasted = dynamic_cast<const IpV6User*>(&second);
    if (otherCasted == nullptr)
        return false;

    return this->Address == otherCasted->Address;
}

RevisionFlags IpV6User::UserKind() const
{
    return RevisionFlags::IpV6User;
}