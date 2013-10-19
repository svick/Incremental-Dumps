#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <memory>
#include "DumpObjects/TextGroup.h"
#include "Diff/DiffWriter.h"

/**
 * Handles reading and writing to text groups.
 */
class TextGroupsManager
{
private:
    std::weak_ptr<Dump> dump;

    /**
     * Current group used for reading or adding (but not both).
     */
    std::unique_ptr<TextGroup> currentGroup;
    /**
     * Current group used for deleting texts from groups.
     */
    std::unique_ptr<TextGroup> currentDeletedGroup;
    /**
     * Whether #currentGroup was modified.
     */
    bool textGroupModified;

    /**
     * Creates and returns new empty group.
     */
    std::unique_ptr<TextGroup> CreateNewGroup();

    /**
     * Ends #currentGroup.
     * If it was modified, compresses it and writes it to the dump.
     */
    void EndGroup(DiffWriter* diffWriter);
    /**
     * Ends #currentDeletedGroup.
     * Recompresses it and writes it to the dump.
     */
    void EndDeletedGroup();
public:
    TextGroupsManager(std::weak_ptr<Dump> dump);

    /**
     * Adds the given @a text to a text group.
     *
     * @returns A pair containing the ID of the group to which the text was added and the ID of the text inside that group.
     */
    std::pair<std::uint32_t, std::uint8_t> AddTextToGroup(const std::string& text);
    /**
     * Writes the current group to the dump if it's full.
     */
    void WriteTextGroupIfFull(DiffWriter* diffWriter);

    /**
     * Deletes the given text from the given group.
     *
     * To avoid unnecessary recompression, deletes from the same group are batched and recompressed together.
     */
    void DeleteTextFromGroup(std::uint32_t textGroupId, std::uint8_t textId);

    /**
     * Returns the given text from the given group.
     */
    std::string GetTextFromGroup(std::uint32_t textGroupId, std::uint8_t textId);

    /**
     * Creates a new text group from a string containing compressed texts in the same format as text group.
     *
     * @returns ID of the new group.
     */
    std::uint32_t ImportTextGroup(const std::string& compressedTexts); 

    /**
     * Completes writing to the dump by writing any open text groups.
     */
    void Complete(DiffWriter* diffWriter);
};