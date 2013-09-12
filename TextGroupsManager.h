#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <memory>
#include "DumpObjects/TextGroup.h"
#include "Diff/DiffWriter.h"

class TextGroupsManager
{
private:
    std::weak_ptr<WritableDump> dump;

    std::unique_ptr<TextGroup> currentGroup;
    std::unique_ptr<TextGroup> currentDeletedGroup;
    bool textGroupModified;

    std::unique_ptr<TextGroup> CreateNewGroup();

    void EndGroup(DiffWriter* diffWriter);
    void EndDeletedGroup();
public:
    TextGroupsManager(std::weak_ptr<WritableDump> dump);

    std::pair<std::uint32_t, std::uint8_t> AddTextToGroup(const std::string& text);
    void WriteTextGroupIfFull(DiffWriter* diffWriter);

    void DeleteTextFromGroup(std::uint32_t textGroupId, std::uint8_t textId);

    std::string GetTextFromGroup(std::uint32_t textGroupId, std::uint8_t textId);

    std::uint32_t ImportTextGroup(const std::string& compressedTexts); 

    void Complete(DiffWriter* diffWriter);
};