#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <memory>
#include "DumpObjects/TextGroup.h"

class TextGroupsManager
{
private:
    std::weak_ptr<WritableDump> dump;

    std::unique_ptr<TextGroup> currentGroup;
    std::unique_ptr<TextGroup> currentDeletedGroup;
    bool textGroupModified;

    void EndDeletedGroup();
public:
    TextGroupsManager(std::weak_ptr<WritableDump> dump);

    std::pair<std::uint32_t, std::uint8_t> AddTextToGroup(const std::string& text);
    void EndGroup();
    void WriteTextGroupIfFull();

    void DeleteTextFromGroup(std::uint32_t textGroupId, std::uint8_t textId);

    std::string GetTextFromGroup(std::uint32_t textGroupId, std::uint8_t textId);

    void Complete();
};