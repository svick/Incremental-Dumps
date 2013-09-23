#pragma once

#include "Change.h"
#include "../../Objects/Page.h"

/**
 * A change that represents adding a new page to the dump.
 *
 * Following revision changes belong to this page.
 */
class NewPageChange : public Change
{
public:
    /**
     * The added page.
     */
    Page page;

    NewPageChange(const Page &page)
        : page(page)
    {}

    static NewPageChange Read(std::istream &stream);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};