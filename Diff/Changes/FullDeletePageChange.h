#pragma once

#include "Change.h"

/**
 * A change that represents deleting a page along with all its revisions.
 */
class FullDeletePageChange : public Change
{
public:
    /**
     * The ID of the deleted page.
     */
    std::uint32_t pageId;

    FullDeletePageChange(std::uint32_t pageId)
        : pageId(pageId)
    {}

    static FullDeletePageChange Read(std::istream &stream);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};