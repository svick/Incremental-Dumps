#pragma once

#include "Change.h"

class FullDeletePageChange : public Change
{
public:
    std::uint32_t pageId;

    FullDeletePageChange(std::uint32_t pageId)
        : pageId(pageId)
    {}

    static FullDeletePageChange Read(std::istream &stream);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};