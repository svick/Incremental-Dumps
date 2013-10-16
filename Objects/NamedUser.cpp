#include "NamedUser.h"
#include "Revision.h"

NamedUser::NamedUser(std::uint32_t userId, std::string userName)
    : User(userId, userName)
{}

bool NamedUser::Equals(const User &second) const
{
    auto otherCasted = dynamic_cast<const NamedUser*>(&second);
    if (otherCasted == nullptr)
        return false;

    return this->UserId == otherCasted->UserId
        && this->UserName == otherCasted->UserName;
}

RevisionFlags NamedUser::UserKind() const
{
    return RevisionFlags::NamedUser;
}