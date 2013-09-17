#pragma once

#include <cstdint>
#include <memory>
#include <string>

using std::string;

enum class RevisionFlags : uint8_t;

class User
{
protected:
    User(std::uint32_t userId, const std::string& userName);
public:
    static std::unique_ptr<User> Create(std::uint32_t userId, const std::string& userName);
    // creates IPv4 or IPv6 anonymous users
    static std::unique_ptr<User> CreateFromIp(const std::string& ipAddress);

    std::uint32_t UserId;
    std::string UserName;

    virtual bool Equals(const User &second) const = 0;

    virtual RevisionFlags UserKind() const = 0;
    virtual ~User() {}
};

bool operator ==(const User &first, const User &second);

bool operator !=(const User &first, const User &second);

// compares pointers that can be null for value equality
bool Equals(const User *first, const User *second);