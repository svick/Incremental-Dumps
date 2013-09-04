#pragma once

#include "DumpObject.h"

class TextGroup : public DumpObject
{
private:
    std::uint32_t textGroupId;
    std::vector<std::string> texts;
    std::string compressedTexts;
    // if a group is not editable, new texts cannot be added, but old texts can still be deleted
    bool isEditable;

    void EnsureCompressed();
    void Load();
    std::vector<std::string> Read(Offset offset);

    bool IsEmpty() const;
protected:
    virtual void WriteInternal() override;
    virtual void UpdateIndex(Offset offset, bool overwrite) override;
public:
    TextGroup(std::weak_ptr<WritableDump> dump, std::uint32_t textGroupId);

    virtual uint32_t NewLength() override;

    std::uint32_t GetTextGroupId() const;

    std::uint8_t AddText(const std::string& text);
    // returns true if the whole group has now been deleted
    bool DeleteText(std::uint8_t textId);
    std::string GetText(std::uint8_t textId) const;

    bool IsFull() const;
};