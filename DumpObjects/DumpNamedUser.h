#pragma once

#include "DumpUser.h"
#include "../Objects/NamedUser.h"

/**
 * Represents NamedUser on the disk.
 */
class DumpNamedUser : public DumpUser
{
private:
    std::shared_ptr<NamedUser> user;
protected:
    virtual void WriteInternal();
public:
    static std::unique_ptr<DumpUser> Read(std::istream &stream);

    DumpNamedUser(std::shared_ptr<NamedUser> user);

    virtual std::shared_ptr<User> GetUser() const OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};