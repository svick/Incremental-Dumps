#pragma once

#include "Change.h"

class PartialDeletePageChange : public Change
{
public:
    std::uint32_t pageId;

    PartialDeletePageChange(std::uint32_t pageId)
        : pageId(pageId)
    {}

    static PartialDeletePageChange Read(std::istream &stream);
    virtual void WriteInternal() override;
    virtual std::uint32_t NewLength() override;
};