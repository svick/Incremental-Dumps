#pragma once

#include <memory>
#include <string>
#include <functional>
#include "User.h"
#include "Timestamp.h"

/** @file */

/**
 * Flags that affect revision.
 *
 * All these flags are together to save space when saved on disk.
 */
enum class RevisionFlags : std::uint8_t
{
    /**
     * No flags are set
     */
    None                = 0x00,

    /**
     * A minor edit
     */
    Minor               = 0x01,
    /**
     * This page has the content model @c wikitext and format @c text/x-wiki
     */
    WikitextModelFormat = 0x02,

    /**
     * The contributor of this revision is a NamedUser
     */
    NamedUser           = 0x04,
    /**
     * The contributor of this revision is an IpV4User
     */
    IpV4User            = 0x08,
    /**
     * The contributor of this revision is an IpV6User
     */
    IpV6User            = 0x10,

    /**
     * The text of this revision was deleted
     */
    TextDeleted         = 0x20,
    /**
     * The comment for this revision was deleted
     */
    CommentDeleted      = 0x40,
    /**
     * The contributor of this revision was deleted
     */
    ContributorDeleted  = 0x80
};

/**
 * Combines two values of RevisionFlags together.
 */
RevisionFlags operator |(RevisionFlags first, RevisionFlags second);
RevisionFlags operator |=(RevisionFlags &first, RevisionFlags second);

/**
 * Checks whether the @a value has the @a flag set.
 */
bool HasFlag(RevisionFlags value, RevisionFlags flag);

/**
 * A revision in the wiki.
 */
class Revision
{
private:
    bool textSet;
    std::string text;
    std::function<std::string()> getTextFunction;
public:
    Revision();

    /**
     * Flags that affect this revision.
     */
    RevisionFlags Flags;
    /**
     * Unique ID of the revision.
     */
    std::uint32_t RevisionId;
    /**
     * ID of the parent revision.
     */
    std::uint32_t ParentId;
    /**
     * The timestamp when this revision was made.
     */
    Timestamp DateTime;
    /**
     * The contributor who made this revision.
     */
    std::shared_ptr<User> Contributor;
    /**
     * Comment describing the revision.
     */
    std::string Comment;

    /**
     * Returns the text of this revision.
     *
     * If the text cannpot be retrieved, throws an exception.
     */
    std::string GetText();
    /**
     * Sets the text of this revision to the given value.
     */
    void SetText(const std::string &text);
    /**
     * Sets a function that can be used to retrieve the text of this revision.
     *
     * This is useful in cases when retrieving the text of the revision is expensive,
     * so it should be done only when the text is actually needed.
     */
    void SetGetText(std::function<std::string()> getTextFunction);
    /**
     * The length of the text of this revision.
     *
     * This field contains useful value only when the text of this revision has been set.
     */
    std::uint32_t TextLength;
    /**
     * ID of the text of this revision.
     *
     * This is used when the text of this revision can be retrieved from the database.
     * In other cases, this field is 0.
     */
    std::uint32_t TextId;

    /**
     * SHA-1 hash of the text of this revision.
     */
    std::string Sha1;
    /**
     * @brief
     * The content model of this revision (for example @c wikitext).
     */
    std::string Model;
    /**
     * The content format of this revision (for example @c text/x-wiki).
     */
    std::string Format;
};

bool operator ==(const Revision &first, const Revision &second);