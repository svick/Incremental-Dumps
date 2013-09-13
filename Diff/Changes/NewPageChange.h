#pragma once

#include "Change.h"
#include "../../Objects/Page.h"

class NewPageChange : public Change
{
public:
    Page page;

    NewPageChange(const Page &page)
        : page(page)
    {}

    static NewPageChange Read(std::istream &stream);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};