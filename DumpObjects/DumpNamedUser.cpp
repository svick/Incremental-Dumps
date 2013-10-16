#include "DumpNamedUser.h"
#include "../Objects/NamedUser.h"

void DumpNamedUser::WriteInternal()
{
    WriteValue(user->UserId);
    WriteValue(user->UserName);
}

std::unique_ptr<DumpUser> DumpNamedUser::Read(std::istream &stream)
{
    std::uint32_t userId = DumpTraits<std::uint32_t>::Read(stream);
    std::string userName = DumpTraits<std::string>::Read(stream);

    return std::unique_ptr<DumpUser>(new DumpNamedUser(std::make_shared<NamedUser>(userId, userName)));
}

DumpNamedUser::DumpNamedUser(std::shared_ptr<NamedUser> user)
    : user(user)
{}

std::shared_ptr<User> DumpNamedUser::GetUser() const
{
    return user;
}

std::uint32_t DumpNamedUser::NewLength()
{
    return ValueSize(user->UserId) + ValueSize(user->UserName);
}