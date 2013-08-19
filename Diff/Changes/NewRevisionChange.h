#pragma once

#include "Change.h"
#include "../../Objects/Revision.h"

class NewRevisionChange : public Change
{
private:
    bool withText;
public:
    Revision revision;
    std::uint8_t modelFormatId;

    NewRevisionChange(const Revision &revision, std::uint8_t modelFormatId, bool withText)
        : revision(revision), modelFormatId(modelFormatId), withText(withText)
    {}

    static NewRevisionChange Read(std::istream &stream, bool withText);
    virtual void WriteInternal() override;
    virtual std::uint32_t NewLength() override;

    virtual void Accept(ChangeVisitor &visitor) override;
};