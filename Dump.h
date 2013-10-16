#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <iostream>
#include <unordered_set>
#include "DumpObjects/FileHeader.h"
#include "DumpObjects/DumpSiteInfo.h"
#include "DumpObjects/TextGroup.h"
#include "Diff/DiffWriter.h"

class WritableDump;

template<typename TKey, typename TValue>
class Index;

class SpaceManager;
class TextGroupsManager;

// TODO: ReadableDump is unusable; either merge it with WritableDump, or make it usable
class ReadableDump
{
protected:
    std::weak_ptr<WritableDump> self;

    ReadableDump(std::unique_ptr<std::iostream> stream);
public:
    // TODO: others should not be able to steal these pointers
    /**
     * The streeam that is used to read and write from this dump.
     */
    std::unique_ptr<std::iostream> stream;
    /**
     * Index of free space in the file.
     *
     * Maps starting offset of each block of free space to its length.
     */
    std::unique_ptr<Index<Offset, std::uint32_t>> spaceIndex;
    /**
     * Index mapping page ID to the offset of the corresponding page object.
     */
    std::unique_ptr<Index<std::uint32_t, Offset>> pageIdIndex;
    /**
     * Index mapping revision ID to the offset of the corresponding revision object.
     */
    std::unique_ptr<Index<std::uint32_t, Offset>> revisionIdIndex;
    /**
     * Index mapping text group ID to the offset of the corresponding text group object.
     */
    std::unique_ptr<Index<std::uint32_t, Offset>> textGroupIdIndex;
    /**
     * Index mapping ID to pair of content model and format strings.
     */
    std::unique_ptr<Index<std::uint8_t, std::pair<std::string, std::string>>> modelFormatIndex;

    /**
     * The site info object for this dump.
     */
    std::unique_ptr<DumpSiteInfo> siteInfo;

    /**
     * Whether this dump is newly created.
     */
    bool isNew;

    ReadableDump(const std::string& fileName);

    /**
     * Returns smart pointer to this instance of dump.
     */
    std::weak_ptr<WritableDump> GetSelf() const;
};

/**
 * This is the main class that represents a dump file on disk.
 * It can be used to read and write to the dump.
 */
class WritableDump : public ReadableDump
{
private:
    static std::unique_ptr<std::iostream> openStream(std::string fileName);

    WritableDump(const std::string& fileName);
    void init(std::shared_ptr<WritableDump> self);

    std::pair<bool, std::vector<std::uint32_t>> DeletePage(
        std::uint32_t pageId, bool fullDelete, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions);
public:
    static const std::string WikitextModel;
    static const std::string WikitextFormat;

    /**
     * Creates a new WritableDump object for a given file.
     * If the file doesn't exist, it will be created.
     */
    static std::shared_ptr<WritableDump> Create(std::string fileName);

    /**
     * File header for this dump.
     */
    FileHeader fileHeader;
    /**
     * Object that allocates and deallocates space in the dump file.
     */
    std::unique_ptr<SpaceManager> spaceManager;
    /**
     * Object that handles reading and writing to text groups.
     */
    std::unique_ptr<TextGroupsManager> textGroupsManager;

    /**
     * Completes writing to the dump.
     * 
     * It's necessary to call this after writing is finished.
     */
    void Complete(DiffWriter* diffWriter);

    /**
     * Deletes the page with the given id from the dump, but not its revisions.
     */
    void DeletePagePartial(std::uint32_t pageId);
    
    /**
     * Deletes the page with the given id from the dump, including its revisions.
     * Revisions contained in @a doNotDeleteRevisions won't be deleted.
     *
     * @returns First member of returned pair indicates whether all revisions of the page were deleted.
     *          If it's @c false, second member contains ids of revisions that were actually deleted.
     */
    std::pair<bool, std::vector<std::uint32_t>> DeletePageFull(
        std::uint32_t pageId, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions = std::unordered_set<std::uint32_t>());

    /**
     * Deletes the revision with the given id from the dump.
     * If the revision is in @a doNotDeleteRevisions, it won't be deleted.
     *
     * @returns Whether the revision was actually deleted.
     */
    bool DeleteRevision(
        std::uint32_t revisionId, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions = std::unordered_set<std::uint32_t>());

    /**
     * Returns id for given combination of content model and format.
     *
     * @param isNew On return, this is @c true if the returned id is newly created
     */
    std::uint8_t GetIdForModelFormat(std::string model, std::string format, bool &isNew);
    
    /**
     * Returns a pair containing content model and format for the given id.
     */
    std::pair<std::string, std::string> GetModelFormat(std::uint8_t id);
};