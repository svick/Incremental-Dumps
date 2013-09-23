#pragma once

#include "DumpUser.h"
#include "../Objects/NamedUser.h"

/**
 * Represents NamedUser on the disk.
 */
class DumpNamedUser : public DumpUser
{
private:
    shared_ptr<NamedUser> user;
protected:
    virtual void WriteInternal();
public:
    static unique_ptr<DumpUser> Read(istream &stream);

    DumpNamedUser(shared_ptr<NamedUser> user);

    virtual shared_ptr<User> GetUser() const OVERRIDE;
    virtual uint32_t NewLength() OVERRIDE;
};