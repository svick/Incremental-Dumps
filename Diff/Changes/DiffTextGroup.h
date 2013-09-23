#pragma once

#include "Change.h"

/**
 * A text group in diff dump.
 *
 * Should be imported as a TextGroup.
 * Following changes with text implicitly reference this text group.
 */
class DiffTextGroup : public Change
{
public:
    /**
     * The group of texts, compressed into a single string.
     *
     * This string is in the same format as TextGroup::compressedTexts.
     */
    const std::string compressedTexts;

    DiffTextGroup(const std::string &compressedTexts);

    static DiffTextGroup Read(std::istream &stream);
    virtual std::uint32_t NewLength() OVERRIDE;
    virtual void WriteInternal() OVERRIDE;
};