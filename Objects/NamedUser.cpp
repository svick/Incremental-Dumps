#include "NamedUser.h"
#include "Revision.h"

NamedUser::NamedUser(std::uint32_t userId, string userName)
    : User(userId, userName)
{}

RevisionFlags NamedUser::UserKind() const
{
    return RevisionFlags::NamedUser;
}