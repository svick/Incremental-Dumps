#include "DumpIpV6User.h"

void DumpIpV6User::WriteInternal()
{
    WriteValue(user->Address);
}

unique_ptr<DumpUser> DumpIpV6User::Read(istream &stream)
{
    auto address = DumpTraits<std::array<std::uint16_t, 8>>::Read(stream);

    return unique_ptr<DumpUser>(new DumpIpV6User(std::make_shared<IpV6User>(address)));
}

DumpIpV6User::DumpIpV6User(shared_ptr<IpV6User> user)
    : user(user)
{}

shared_ptr<User> DumpIpV6User::GetUser() const
{
    return user;
}

uint32_t DumpIpV6User::NewLength()
{
    return ValueSize(user->Address);
}