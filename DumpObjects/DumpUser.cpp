#include "DumpIpV4User.h"
#include "DumpIpV6User.h"
#include "DumpNamedUser.h"

using std::dynamic_pointer_cast;

unique_ptr<DumpUser> DumpUser::Create(shared_ptr<User> user)
{
    auto ipV4User = dynamic_pointer_cast<IpV4User>(user);
    if (ipV4User != nullptr)
        return unique_ptr<DumpUser>(new DumpIpV4User(ipV4User));
    
    auto ipV6User = dynamic_pointer_cast<IpV6User>(user);
    if (ipV6User != nullptr)
        return unique_ptr<DumpUser>(new DumpIpV6User(ipV6User));

    auto namedUser = dynamic_pointer_cast<NamedUser>(user);
    if (namedUser != nullptr)
        return unique_ptr<DumpUser>(new DumpNamedUser(namedUser));

    throw DumpException();
}

unique_ptr<DumpUser> DumpUser::Read(RevisionFlags flags, istream &stream)
{
    if (HasFlag(flags, RevisionFlags::IpV4User))
        return DumpIpV4User::Read(stream);
    if (HasFlag(flags, RevisionFlags::IpV6User))
        return DumpIpV6User::Read(stream);
    if (HasFlag(flags, RevisionFlags::NamedUser))
        return DumpNamedUser::Read(stream);
    throw new DumpException();
}

void DumpUser::Write(ostream *stream)
{
    this->stream = stream;
    WriteInternal();
    this->stream = nullptr;
}