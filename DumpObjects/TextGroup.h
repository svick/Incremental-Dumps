#pragma once

#include "DumpObject.h"

class TextGroup : public DumpObject
{
private:
    std::uint32_t textGroupId;
    std::vector<std::string> texts;
    // if a group is not editable, new texts cannot be added, but old texts can still be deleted
    bool isEditable;

    void EnsureCompressed();
    void Load();
    std::vector<std::string> Read(Offset offset);

    bool IsEmpty() const;
protected:
    virtual void WriteInternal() OVERRIDE;
    virtual void UpdateIndex(Offset offset, bool overwrite) OVERRIDE;
public:
    std::string compressedTexts;

    TextGroup(std::weak_ptr<WritableDump> dump, std::uint32_t textGroupId);

    virtual uint32_t NewLength() OVERRIDE;

    std::uint32_t GetTextGroupId() const;

    std::uint8_t AddText(const std::string& text);
    // returns true if the whole group has now been deleted
    bool DeleteText(std::uint8_t textId);
    std::string GetText(std::uint8_t textId) const;

    void SetCompressedTexts(const std::string& compressedTexts);

    bool IsFull() const;
};