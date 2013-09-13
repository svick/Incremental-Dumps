#pragma once

#include "User.h"
#include "../common.h"

class NamedUser : public User
{
public:
    NamedUser(std::uint32_t userId, string userName);

    virtual bool Equals(const User &second) const OVERRIDE;

    virtual RevisionFlags UserKind() const OVERRIDE;
};