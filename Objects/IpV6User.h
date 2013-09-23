#pragma once

#include <array>
#include "User.h"
#include "../common.h"

/**
 * Anonymous user with IPv6 address.
 */
class IpV6User : public User
{
public:
    /**
     * Tries to parse given string as IPv6 address.
     *
     * @param success Is @c true on return if the string was parsed successfully.
     * @returns The parsed IPv6 adress.
     */
    static std::array<std::uint16_t, 8> TryParseAddress(const std::string& address, bool &success);
    /**
     * Parses given string as IPv6 address.
     *
     * If the string couldn't be parsed, throws exception.
     *
     * @returns The parsed IPv6 adress.
     */
    static std::array<std::uint16_t, 8> ParseAddress(const std::string& address);
    /**
     * Converts IPv6 address to string.
     */
    static std::string AddressToString(std::array<std::uint16_t, 8> address);

    IpV6User(const std::string& stringAddress);
    IpV6User(const std::string& stringAddress, std::array<std::uint16_t, 8> parsedAddress);
    IpV6User(std::array<std::uint16_t, 8> parsedAddress);

    /**
     * IPv6 address of this user.
     */
    std::array<std::uint16_t, 8> Address;

    virtual bool Equals(const User &second) const OVERRIDE;

    virtual RevisionFlags UserKind() const OVERRIDE;
};