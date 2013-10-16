#pragma once

#include "DumpUser.h"
#include "../Objects/IpV6User.h"

/**
 * Represents IpV6User on the disk.
 */
class DumpIpV6User : public DumpUser
{
private:
    std::shared_ptr<IpV6User> user;
protected:
    virtual void WriteInternal();
public:
    static std::unique_ptr<DumpUser> Read(std::istream &stream);

    DumpIpV6User(std::shared_ptr<IpV6User> user);

    virtual std::shared_ptr<User> GetUser() const OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};