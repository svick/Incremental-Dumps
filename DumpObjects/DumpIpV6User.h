#pragma once

#include "DumpUser.h"
#include "../Objects/IpV6User.h"

class DumpIpV6User : public DumpUser
{
protected:
    virtual void WriteInternal();
public:
    static unique_ptr<DumpUser> Read(istream &stream);

    shared_ptr<IpV6User> user;

    DumpIpV6User(shared_ptr<IpV6User> user);

    virtual shared_ptr<User> GetUser() const OVERRIDE;
    virtual uint32_t NewLength() OVERRIDE;
};