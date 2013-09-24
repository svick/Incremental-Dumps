#pragma once

#include "Change.h"

/**
 * A change that represents deleting a revision.
 *
 * In the case when a revision is deleted but its page isn't,
 * this change has to follow the corresponding page change.
 */
class DeleteRevisionChange : public Change
{
public:
    /**
     * The ID of the deleted revision.
     */
    std::uint32_t revisionId;

    DeleteRevisionChange(std::uint32_t revisionId)
        : revisionId(revisionId)
    {}

    static DeleteRevisionChange Read(std::istream &stream);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};