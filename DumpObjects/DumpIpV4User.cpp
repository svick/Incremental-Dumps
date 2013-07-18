#include "DumpIpV4User.h"

void DumpIpV4User::WriteInternal()
{
    WriteValue(user->Address);
}

unique_ptr<DumpUser> DumpIpV4User::Read(istream &stream)
{
    int address = DumpTraits<uint32_t>::Read(stream);

    return unique_ptr<DumpUser>(new DumpIpV4User(shared_ptr<IpV4User>(new IpV4User(address))));
}

DumpIpV4User::DumpIpV4User(shared_ptr<IpV4User> user)
    : user(user)
{}

shared_ptr<User> DumpIpV4User::GetUser() const
{
    return user;
}

uint32_t DumpIpV4User::NewLength() const
{
    return ValueSize(user->Address);
}