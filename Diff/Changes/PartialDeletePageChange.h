#pragma once

#include "Change.h"

/**
 * A change that represents deleting a page, but without deleting its revisions.
 */
class PartialDeletePageChange : public Change
{
public:
    /**
     * The ID of the deleted page.
     */
    std::uint32_t pageId;

    PartialDeletePageChange(std::uint32_t pageId)
        : pageId(pageId)
    {}

    static PartialDeletePageChange Read(std::istream &stream);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};