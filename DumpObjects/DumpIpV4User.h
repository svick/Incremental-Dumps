#pragma once

#include "DumpUser.h"
#include "../Objects/IpV4User.h"

/**
 * Represents IpV4User on the disk.
 */
class DumpIpV4User : public DumpUser
{
private:
    std::shared_ptr<IpV4User> user;
protected:
    virtual void WriteInternal();
public:
    static std::unique_ptr<DumpUser> Read(std::istream &stream);

    DumpIpV4User(std::shared_ptr<IpV4User> user);

    virtual std::shared_ptr<User> GetUser() const OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};