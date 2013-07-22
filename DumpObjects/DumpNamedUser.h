#pragma once

#include "DumpUser.h"

class DumpNamedUser : public DumpUser
{
protected:
    virtual void WriteInternal();
public:
    static unique_ptr<DumpUser> Read(istream &stream);

    shared_ptr<User> user;

    DumpNamedUser(shared_ptr<User> user);

    virtual shared_ptr<User> GetUser() const override;
    virtual uint32_t NewLength() override;
};