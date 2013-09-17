#include "User.h"
#include "IpV4User.h"
#include "IpV6User.h"
#include "NamedUser.h"
#include "../DumpException.h"

std::unique_ptr<User> TryCreateFromIp(const std::string& ipAddress, bool &success)
{
    std::uint32_t ipV4 = IpV4User::TryParseAddress(ipAddress, success);
    if (success)
        return std::unique_ptr<User>(new IpV4User(ipAddress, ipV4));

    auto ipV6 = IpV6User::TryParseAddress(ipAddress, success);
    if (success)
        return std::unique_ptr<User>(new IpV6User(ipAddress, ipV6));

    success = false;
    return nullptr;
}

std::unique_ptr<User> User::Create(std::uint32_t userId, const std::string& userName)
{
    if (userId == 0)
    {
        bool success;

        auto ipUser = TryCreateFromIp(userName, success);

        if (success)
            return ipUser;
    }

    return std::unique_ptr<User>(new NamedUser(userId, userName));
}

std::unique_ptr<User> User::CreateFromIp(const std::string& ipAddress)
{
    bool success;

    auto ipUser = TryCreateFromIp(ipAddress, success);

    if (!success)
        throw DumpException();

    return ipUser;
}

User::User(std::uint32_t userId, const std::string& userName)
    : UserId(userId), UserName(userName)
{}

bool operator ==(const User &first, const User &second)
{
    return first.Equals(second);
}

bool operator !=(const User &first, const User &second)
{
    return !(first == second);
}

bool Equals(const User *first, const User *second)
{
    if (first == nullptr && second == nullptr)
        return true;

    if (first != nullptr && second != nullptr)
        return *first == *second;

    return false;
}