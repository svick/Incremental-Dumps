#include "DumpNamedUser.h"
#include "../Objects/NamedUser.h"

void DumpNamedUser::WriteInternal()
{
    WriteValue(user->UserId);
    WriteValue(user->UserName);
}

unique_ptr<DumpUser> DumpNamedUser::Read(istream &stream)
{
    uint32_t userId = DumpTraits<uint32_t>::Read(stream);
    string userName = DumpTraits<string>::Read(stream);

    return unique_ptr<DumpUser>(new DumpNamedUser(std::make_shared<NamedUser>(userId, userName)));
}

DumpNamedUser::DumpNamedUser(shared_ptr<NamedUser> user)
    : user(user)
{}

shared_ptr<User> DumpNamedUser::GetUser() const
{
    return user;
}

uint32_t DumpNamedUser::NewLength()
{
    return ValueSize(user->UserId) + ValueSize(user->UserName);
}