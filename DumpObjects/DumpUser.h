#pragma once

#include "DumpObject.h"
#include "../Objects/User.h"
#include "../Objects/Revision.h"

using std::shared_ptr;
using std::istream;

class DumpUser : public DumpObjectBase
{
protected:
    using DumpObjectBase::WriteValue;
    using DumpObjectBase::ValueSize;

    virtual void WriteInternal() OVERRIDE = 0;
public:
    static unique_ptr<DumpUser> Create(shared_ptr<User> user);
    static unique_ptr<DumpUser> Read(RevisionFlags flags, istream &stream);

    void Write(ostream *stream);
    virtual shared_ptr<User> GetUser() const = 0;
    virtual uint32_t NewLength() OVERRIDE = 0;
};