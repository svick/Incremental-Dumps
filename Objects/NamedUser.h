#pragma once

#include "User.h"

class NamedUser : public User
{
public:
    NamedUser(std::uint32_t userId, string userName);

    virtual bool Equals(const User &second) const override;

    virtual RevisionFlags UserKind() const override;
};