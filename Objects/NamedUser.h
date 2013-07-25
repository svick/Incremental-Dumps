#pragma once

#include "User.h"

class NamedUser : public User
{
public:
    NamedUser(std::uint32_t userId, string userName);

    virtual RevisionFlags UserKind() const override;
};