#pragma once

#include "Change.h"

class DeleteRevisionChange : public Change
{
public:
    std::uint32_t revisionId;

    DeleteRevisionChange(std::uint32_t revisionId)
        : revisionId(revisionId)
    {}

    static DeleteRevisionChange Read(std::istream &stream);
    virtual void WriteInternal() override;
    virtual std::uint32_t NewLength() override;

    virtual void Accept(ChangeVisitor &visitor) override;
};