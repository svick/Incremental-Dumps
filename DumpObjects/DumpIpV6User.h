#pragma once

#include "DumpUser.h"
#include "../Objects/IpV6User.h"

/**
 * Represents IpV6User on the disk.
 */
class DumpIpV6User : public DumpUser
{
private:
    shared_ptr<IpV6User> user;
protected:
    virtual void WriteInternal();
public:
    static unique_ptr<DumpUser> Read(istream &stream);

    DumpIpV6User(shared_ptr<IpV6User> user);

    virtual shared_ptr<User> GetUser() const OVERRIDE;
    virtual uint32_t NewLength() OVERRIDE;
};