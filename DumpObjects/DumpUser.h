#pragma once

#include "DumpObject.h"
#include "../Objects/User.h"
#include "../Objects/Revision.h"

using std::shared_ptr;
using std::istream;

/**
 * Abstract base class that represets a types derived from User on the disk.
 */
class DumpUser : public DumpObjectBase
{
protected:
    using DumpObjectBase::WriteValue;
    using DumpObjectBase::ValueSize;

    virtual void WriteInternal() OVERRIDE = 0;
public:
    /**
     * Creates the right kind of DumpUser from the given @a user.
     */
    static unique_ptr<DumpUser> Create(shared_ptr<User> user);
    /**
     * Reads DumpUser from the @a stream, considering given @a flags.
     *
     * The @a flags contain information about what kind of user to read.
     */
    static unique_ptr<DumpUser> Read(RevisionFlags flags, istream &stream);

    /**
     * Writes this user to the given @a stream.
     */
    void Write(ostream *stream);
    /**
     * Gets the user this object represents.
     */
    virtual shared_ptr<User> GetUser() const = 0;
    virtual uint32_t NewLength() OVERRIDE = 0;
};