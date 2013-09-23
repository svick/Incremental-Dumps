#pragma once

#include "DumpUser.h"
#include "../Objects/IpV4User.h"

/**
 * Represents IpV4User on the disk.
 */
class DumpIpV4User : public DumpUser
{
private:
    shared_ptr<IpV4User> user;
protected:
    virtual void WriteInternal();
public:
    static unique_ptr<DumpUser> Read(istream &stream);

    DumpIpV4User(shared_ptr<IpV4User> user);

    virtual shared_ptr<User> GetUser() const OVERRIDE;
    virtual uint32_t NewLength() OVERRIDE;
};