#pragma once

#include "User.h"
#include "../common.h"

class IpV4User : public User
{
public:
    static uint32_t TryParseAddress(string address, bool &success);
    static uint32_t ParseAddress(string address);
    static string AddressToString(uint32_t address);

    IpV4User(string stringAddress);
    IpV4User(string stringAddress, uint32_t parsedAddress);
    IpV4User(uint32_t parsedAddress);

    uint32_t Address;

    virtual bool Equals(const User &second) const OVERRIDE;

    virtual RevisionFlags UserKind() const OVERRIDE;
};