#pragma once

#include "User.h"

class IpV4User : public User
{
public:
    static uint32_t ParseAddress(string address);
    static string AddressToString(uint32_t address);

    IpV4User(string stringAddress);
    IpV4User(string stringAddress, uint32_t parsedAddress);
    IpV4User(uint32_t parsedAddress);

    uint32_t Address;

    virtual RevisionFlags UserKind() const override;
};