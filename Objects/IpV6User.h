#pragma once

#include <array>
#include "User.h"

class IpV6User : public User
{
public:
    static std::array<uint16_t, 8> TryParseAddress(string address, bool &success);
    static std::array<uint16_t, 8> ParseAddress(string address);
    static string AddressToString(std::array<uint16_t, 8> address);

    IpV6User(string stringAddress);
    IpV6User(string stringAddress, std::array<uint16_t, 8> parsedAddress);
    IpV6User(std::array<uint16_t, 8> parsedAddress);

    std::array<uint16_t, 8> Address;

    virtual bool Equals(const User &second) const override;

    virtual RevisionFlags UserKind() const override;
};