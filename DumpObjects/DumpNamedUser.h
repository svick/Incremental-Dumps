#pragma once

#include "DumpUser.h"
#include "../Objects/NamedUser.h"

class DumpNamedUser : public DumpUser
{
protected:
    virtual void WriteInternal();
public:
    static unique_ptr<DumpUser> Read(istream &stream);

    shared_ptr<NamedUser> user;

    DumpNamedUser(shared_ptr<NamedUser> user);

    virtual shared_ptr<User> GetUser() const OVERRIDE;
    virtual uint32_t NewLength() OVERRIDE;
};