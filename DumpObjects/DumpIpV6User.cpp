#include "DumpIpV6User.h"

void DumpIpV6User::WriteInternal()
{
    WriteValue(user->Address);
}

std::unique_ptr<DumpUser> DumpIpV6User::Read(std::istream &stream)
{
    auto address = DumpTraits<std::array<std::uint16_t, 8>>::Read(stream);

    return std::unique_ptr<DumpUser>(new DumpIpV6User(std::make_shared<IpV6User>(address)));
}

DumpIpV6User::DumpIpV6User(std::shared_ptr<IpV6User> user)
    : user(user)
{}

std::shared_ptr<User> DumpIpV6User::GetUser() const
{
    return user;
}

std::uint32_t DumpIpV6User::NewLength()
{
    return ValueSize(user->Address);
}