#pragma once

#include <cstdint>
#include <memory>
#include <string>

using std::uint32_t;
using std::unique_ptr;
using std::string;

enum class RevisionFlags : uint8_t;

// TODO: create class NamedUser?
class User
{
public:
    static unique_ptr<User> Create(uint32_t userId, string userName);

    User(uint32_t userId, string userName);

    uint32_t UserId;
    string UserName;

    virtual RevisionFlags UserKind() const;
    virtual ~User();
};