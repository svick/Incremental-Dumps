#pragma once

#include <cstdint>
#include <memory>
#include <string>

using std::string;

enum class RevisionFlags : uint8_t;

/** @file */

/**
* Abstract base class for various kinds of users.
*/
class User
{
protected:
    User(std::uint32_t userId, const std::string& userName);
public:
    /**
     * Factory method for creating a user.
     *
     * Returns NamedUser, IpV4User or IpV6User, depending on parameters.
     */
    static std::unique_ptr<User> Create(std::uint32_t userId, const std::string& userName);
    /**
    * Factory method for creating an anonymous user based on IP address.
    *
    * Returns either IpV4User or IpV6User, depending on the kind of IP address given.
    * If the address couldn't be parsed, throws an exception.
    */
    static std::unique_ptr<User> CreateFromIp(const std::string& ipAddress);

    /**
     * ID of the user.
     *
     * Is 0 for anonymous users.
     */
    std::uint32_t UserId;
    /**
     * The name of the user.
     *
     * For anonymous users, this is string representation of their IP adress.
     */
    std::string UserName;

    /**
     * Compares two users for equality.
     *
     * This is a virtual method, so that each kind of user can be compared.
     */
    virtual bool Equals(const User &second) const = 0;

    /**
     * Returns the flag that has to be set on revision when its contributor is this user.
     */
    virtual RevisionFlags UserKind() const = 0;

    virtual ~User() {}
};

bool operator ==(const User &first, const User &second);

bool operator !=(const User &first, const User &second);

/**
 * Compares pointers that can be null for value equality
 */
bool Equals(const User *first, const User *second);