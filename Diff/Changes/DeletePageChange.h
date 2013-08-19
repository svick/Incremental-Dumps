#pragma once

#include "Change.h"

class DeletePageChange : public Change
{
public:
    std::uint32_t pageId;

    DeletePageChange(std::uint32_t pageId)
        : pageId(pageId)
    {}

    static DeletePageChange Read(std::istream &stream);
    virtual void WriteInternal() override;
    virtual std::uint32_t NewLength() override;

    virtual void Accept(ChangeVisitor &visitor) override;
};