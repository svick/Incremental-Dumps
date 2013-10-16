#include "DumpIpV4User.h"
#include "DumpIpV6User.h"
#include "DumpNamedUser.h"

std::unique_ptr<DumpUser> DumpUser::Create(std::shared_ptr<User> user)
{
    auto ipV4User = std::dynamic_pointer_cast<IpV4User>(user);
    if (ipV4User != nullptr)
        return std::unique_ptr<DumpUser>(new DumpIpV4User(ipV4User));
    
    auto ipV6User = std::dynamic_pointer_cast<IpV6User>(user);
    if (ipV6User != nullptr)
        return std::unique_ptr<DumpUser>(new DumpIpV6User(ipV6User));

    auto namedUser = std::dynamic_pointer_cast<NamedUser>(user);
    if (namedUser != nullptr)
        return std::unique_ptr<DumpUser>(new DumpNamedUser(namedUser));

    throw DumpException();
}

std::unique_ptr<DumpUser> DumpUser::Read(RevisionFlags flags, std::istream &stream)
{
    if (HasFlag(flags, RevisionFlags::IpV4User))
        return DumpIpV4User::Read(stream);
    if (HasFlag(flags, RevisionFlags::IpV6User))
        return DumpIpV6User::Read(stream);
    if (HasFlag(flags, RevisionFlags::NamedUser))
        return DumpNamedUser::Read(stream);
    throw new DumpException();
}

void DumpUser::Write(std::ostream *stream)
{
    this->stream = stream;
    WriteInternal();
    this->stream = nullptr;
}