#pragma once

#include "Change.h"
#include "../../Objects/Revision.h"

class NewRevisionChange : public Change
{
private:
    bool withText;

    std::string compressedText;
    bool compressedTextSet;

    void EnsureCompressed();
public:
    Revision revision;
    std::uint8_t modelFormatId;

    NewRevisionChange(const Revision &revision, std::uint8_t modelFormatId, bool withText)
        : revision(revision), modelFormatId(modelFormatId), withText(withText), compressedTextSet(false)
    {}

    static NewRevisionChange Read(std::istream &stream, bool withText);
    virtual void WriteInternal() override;
    virtual std::uint32_t NewLength() override;
};