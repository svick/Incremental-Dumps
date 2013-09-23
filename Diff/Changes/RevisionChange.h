#pragma once

#include "Change.h"
#include "../../Objects/Revision.h"

/** @file */

/**
* Flags that indicate which parts of revision changed.
*/
enum class RevisionChangeFlags : std::uint8_t
{
    NoChanges   = 0x00,

    Flags       = 0x01,
    ParentId    = 0x02,
    DateTime    = 0x04,
    Contributor = 0x08,
    Comment     = 0x10,
    Text        = 0x20,
    ModelFormat = 0x40
};

RevisionChangeFlags operator &(RevisionChangeFlags first, RevisionChangeFlags second);
RevisionChangeFlags operator |(RevisionChangeFlags first, RevisionChangeFlags second);
RevisionChangeFlags operator |=(RevisionChangeFlags &first, RevisionChangeFlags second);
bool HasFlag(RevisionChangeFlags value, RevisionChangeFlags flag);

/**
 * A change that represents change in a revision in the dump.
 *
 * This usually means that some fields of the revision were deleted or undeleted.
 */
class RevisionChange : public Change
{
private:
    bool withText;

    RevisionChange(bool withText);
public:
    /**
     * Which parts of the revision changed.
     */
    RevisionChangeFlags flags;

    /**
     * Incomplete Revision object that contains only the changed fields.
     */
    Revision revisionChanges;
    /**
     * The ID of the combination of content model and format (if it changed).
     */
    std::uint8_t newRevisionModelFormatId;

    /**
     * ID of the text of the revision (if it changed).
     *
     * The text is in the text group from the last DiffTextGroup.
     */
    std::uint8_t textId;

    RevisionChange(
        const Revision &oldRevision, Revision &newRevision,
        std::uint8_t newRevisionModelFormatId, bool withText, std::uint8_t textId);

    static RevisionChange Read(std::istream &stream, bool withText);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};