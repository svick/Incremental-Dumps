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

    std::uint8_t textId;

    NewRevisionChange(const Revision &revision, std::uint8_t modelFormatId, bool withText, std::uint8_t textId)
        : revision(revision), modelFormatId(modelFormatId), withText(withText), textId(textId)
    {}

    static NewRevisionChange Read(std::istream &stream, bool withText);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};