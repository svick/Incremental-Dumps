#pragma once

#include "User.h"
#include "../common.h"

/**
 * Anonymous user with IPv4 address.
 */
class IpV4User : public User
{
public:
    /**
     * Tries to parse given string as IPv4 address.
     *
     * @param success Is @c true on return if the string was parsed successfully.
     * @returns The parsed IPv4 adress.
     */
    static std::uint32_t TryParseAddress(std::string address, bool &success);
    /**
     * Parses given string as IPv4 address.
     *
     * If the string couldn't be parsed, throws exception.
     *
     * @returns The parsed IPv4 adress.
     */
    static std::uint32_t ParseAddress(std::string address);
    /**
     * Converts IPv4 address to string.
     */
    static std::string AddressToString(std::uint32_t address);

    IpV4User(std::string stringAddress);
    IpV4User(std::string stringAddress, std::uint32_t parsedAddress);
    IpV4User(std::uint32_t parsedAddress);

    /**
     * IPv4 address of this user.
     */
    std::uint32_t Address;

    virtual bool Equals(const User &second) const OVERRIDE;

    virtual RevisionFlags UserKind() const OVERRIDE;
};