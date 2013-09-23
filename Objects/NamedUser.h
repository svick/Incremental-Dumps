#pragma once

#include "User.h"
#include "../common.h"

/**
 * Represents a user that is not IpV4User or IpV6User.
 *
 * This includes normal logged-in users and historical anomalies,
 * like anonymous users with partial IP addresses.
 */
class NamedUser : public User
{
public:
    NamedUser(std::uint32_t userId, string userName);

    virtual bool Equals(const User &second) const OVERRIDE;

    virtual RevisionFlags UserKind() const OVERRIDE;
};