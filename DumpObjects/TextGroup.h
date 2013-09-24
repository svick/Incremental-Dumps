#pragma once

#include "DumpObject.h"

/**
 * A group of texts that is compressed together.
 *
 * Each group can contain at most 256 texts.
 * A text can't contain the null byte (@c '\0') and it also can't be just UTF-8 encoded U+FFFF.
 */
class TextGroup : public DumpObject
{
private:
    std::uint32_t textGroupId;
    std::vector<std::string> texts;
    /**
     * If a group is not editable, new texts cannot be added, but old texts can still be deleted
     */
    bool isEditable;

    /**
     * A single string that contains all the texts compressed together.
     */
    std::string compressedTexts;

    void EnsureCompressed();
    void Load();
    std::vector<std::string> Read(Offset offset, std::uint32_t& length);

    bool IsEmpty() const;

protected:
    virtual void WriteInternal() OVERRIDE;
    virtual void UpdateIndex(bool overwrite) OVERRIDE;
public:

    TextGroup(std::weak_ptr<WritableDump> dump, std::uint32_t textGroupId);

    virtual uint32_t NewLength() OVERRIDE;

    /**
     * Returns the ID of this group.
     */
    std::uint32_t GetTextGroupId() const;

    /**
     * Adds the given @a text to this group.
     * @returns ID of the added text inside this group.
     */
    std::uint8_t AddText(const std::string& text);
    /**
     * Deletes text with the given ID.
     *
     * @returns @c true if the whole group has now been deleted
     */
    bool DeleteText(std::uint8_t textId);
    /**
     * Returns text with the given ID.
     */
    std::string GetText(std::uint8_t textId) const;

    /**
     * Sets #compressedTexts to the given value.
     *
     * Can't be called if #compressedTexts has already been set
     * or if some texts were already added to the group.
     */
    void SetCompressedTexts(const std::string& compressedTexts);
    /**
     * Returns #compressedTexts.
     */
    const std::string& GetCompressedTexts() const;

    /**
     * Whether this group is full.
     *
     * It's not possible to add new texts to a full group.
     * (And deleting from a full group does *not* make it non-full.)
     */
    bool IsFull() const;
};