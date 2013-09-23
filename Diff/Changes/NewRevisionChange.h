#pragma once

#include "Change.h"
#include "../../Objects/Revision.h"

/**
 * A change that represents adding new revision to the dump.
 *
 * The added revision belongs to the page from the last page change.
 */
class NewRevisionChange : public Change
{
private:
    bool withText;
public:
    /**
     * The added revision.
     */
    Revision revision;
    /**
     * ID of the combination of content model and format of the added revision.
     */
    std::uint8_t modelFormatId;

    /**
     * ID of the text of the added revision.
     *
     * The text is in the text group from the last DiffTextGroup.
     */
    std::uint8_t textId;

    NewRevisionChange(const Revision &revision, std::uint8_t modelFormatId, bool withText, std::uint8_t textId)
        : revision(revision), modelFormatId(modelFormatId), withText(withText), textId(textId)
    {}

    static NewRevisionChange Read(std::istream &stream, bool withText);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};