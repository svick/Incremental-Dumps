#include "DumpNamedUser.h"

void DumpNamedUser::WriteInternal()
{
    WriteValue(user->UserId);
    WriteValue(user->UserName);
}

unique_ptr<DumpUser> DumpNamedUser::Read(istream &stream)
{
    int userId = DumpTraits<uint32_t>::Read(stream);
    string userName = DumpTraits<string>::Read(stream);

    return unique_ptr<DumpUser>(new DumpNamedUser(shared_ptr<User>(new User(userId, userName))));
}

DumpNamedUser::DumpNamedUser(shared_ptr<User> user)
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