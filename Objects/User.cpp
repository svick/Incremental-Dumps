#include "User.h"
#include "IpV4User.h"

unique_ptr<User> User::Create(uint32_t userId, string userName)
{
    User *result;
    uint32_t ipV4 = IpV4User::ParseAddress(userName);
    if (ipV4 != 0)
        result = new IpV4User(userName, ipV4);
    else
        result = new User(userId, userName);

    return unique_ptr<User>(result);
}

User::User(uint32_t userId, string userName)
    : UserId(userId), UserName(userName)
{}

User::~User()
{}