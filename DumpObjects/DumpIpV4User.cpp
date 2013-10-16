#include "DumpIpV4User.h"

void DumpIpV4User::WriteInternal()
{
    WriteValue(user->Address);
}

std::unique_ptr<DumpUser> DumpIpV4User::Read(std::istream &stream)
{
    int address = DumpTraits<uint32_t>::Read(stream);

    return std::unique_ptr<DumpUser>(new DumpIpV4User(std::shared_ptr<IpV4User>(new IpV4User(address))));
}

DumpIpV4User::DumpIpV4User(std::shared_ptr<IpV4User> user)
    : user(user)
{}

std::shared_ptr<User> DumpIpV4User::GetUser() const
{
    return user;
}

std::uint32_t DumpIpV4User::NewLength()
{
    return ValueSize(user->Address);
}