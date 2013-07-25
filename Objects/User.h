#pragma once

#include <cstdint>
#include <memory>
#include <string>

using std::uint32_t;
using std::unique_ptr;
using std::string;

enum class RevisionFlags : uint8_t;

class User
{
protected:
    User(uint32_t userId, string userName);
public:
    static unique_ptr<User> Create(uint32_t userId, string userName);
    // creates IPv4 or IPv6 anonymous users
    static unique_ptr<User> CreateFromIp(string ipAddress);

    uint32_t UserId;
    string UserName;

    virtual RevisionFlags UserKind() const = 0;
    virtual ~User() {}
};