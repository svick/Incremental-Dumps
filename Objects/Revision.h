#pragma once

#include <memory>
#include <string>
#include "User.h"
#include "Timestamp.h"

using std::shared_ptr;
using std::string;

enum class RevisionFlags : uint8_t
{
    None      = 0x00,

    Minor     = 0x01,
    WikitextModelFormat = 0x02,

    NamedUser = 0x04,
    IpV4User  = 0x08,
    IpV6User  = 0x10,

    TextDeleted = 0x20,
    CommentDeleted = 0x40,
    ContributorDeleted = 0x80
};

RevisionFlags operator |(RevisionFlags first, RevisionFlags second);
RevisionFlags operator |=(RevisionFlags &first, RevisionFlags second);
bool HasFlag(RevisionFlags value, RevisionFlags flag);

class Revision
{
public:
    Revision();

    RevisionFlags Flags;
    uint32_t RevisionId;
    uint32_t ParentId;
    Timestamp DateTime;
    shared_ptr<User> Contributor;
    string Comment;
    string Text;
    string Sha1;
    string Model;
    string Format;
};