#pragma once

#include "DumpUser.h"
#include "../Objects/IpV4User.h"

class DumpIpV4User : public DumpUser
{
protected:
    virtual void WriteInternal();
public:
    static unique_ptr<DumpUser> Read(istream &stream);

    shared_ptr<IpV4User> user;

    DumpIpV4User(shared_ptr<IpV4User> user);

    virtual shared_ptr<User> GetUser() const override;
    virtual uint32_t NewLength() override;
};