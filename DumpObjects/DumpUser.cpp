#include "DumpUser.h"
#include "DumpIpV4User.h"
#include "DumpNamedUser.h"

using std::dynamic_pointer_cast;

unique_ptr<DumpUser> DumpUser::Create(shared_ptr<User> user)
{
    DumpUser *result;
    auto ipV4User = dynamic_pointer_cast<IpV4User>(user);
    if (ipV4User != nullptr)
        result = new DumpIpV4User(ipV4User);
    else
        result = new DumpNamedUser(user);

    return unique_ptr<DumpUser>(result);
}

unique_ptr<DumpUser> DumpUser::Read(RevisionFlags flags, istream &stream)
{
    if (HasFlag(flags, RevisionFlags::IpV4User))
    {
        return DumpIpV4User::Read(stream);
    }
    else if (HasFlag(flags, RevisionFlags::NamedUser))
    {
        return DumpNamedUser::Read(stream);
    }
    else
    {
        throw new DumpException();
    }
}

void DumpUser::Write(ostream *stream)
{
    this->stream = stream;
    WriteInternal();
    this->stream = nullptr;
}