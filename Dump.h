#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <iostream>
#include <unordered_set>
#include "DumpObjects/FileHeader.h"
#include "DumpObjects/DumpSiteInfo.h"
#include "DumpObjects/TextGroup.h"

using std::int32_t;
using std::int64_t;
using std::unique_ptr;
using std::shared_ptr;
using std::string;
using std::iostream;

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
    // TODO: others should not be able to steal these
    std::unique_ptr<std::iostream> stream;
    std::unique_ptr<Index<Offset, std::uint32_t>> spaceIndex;
    std::unique_ptr<Index<std::uint32_t, Offset>> pageIdIndex;
    std::unique_ptr<Index<std::uint32_t, Offset>> revisionIdIndex;
    std::unique_ptr<Index<std::uint32_t, Offset>> textGroupIdIndex;
    std::unique_ptr<Index<std::uint8_t, std::pair<std::string, std::string>>> modelFormatIndex;

    std::unique_ptr<DumpSiteInfo> siteInfo;

    bool isNew;

    ReadableDump(std::string fileName);

    std::weak_ptr<WritableDump> GetSelf() const;
};

class WritableDump : public ReadableDump
{
private:
    static std::unique_ptr<std::iostream> openStream(std::string fileName);

    WritableDump(string fileName);
    void init(std::shared_ptr<WritableDump> self);

    std::pair<bool, std::vector<std::uint32_t>> DeletePage(
        std::uint32_t pageId, bool fullDelete, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions);
public:
    static const std::string WikitextModel;
    static const std::string WikitextFormat;

    static shared_ptr<WritableDump> Create(string fileName);

    FileHeader fileHeader;
    std::unique_ptr<SpaceManager> spaceManager;
    std::unique_ptr<TextGroupsManager> textGroupsManager;

    // it's necessary to call this after writing is finished
    void Complete();

    void DeletePagePartial(std::uint32_t pageId);
    // also deletes revisions of the given page
    // first member of returned pair indicates whether all revisions of the page were deleted
    // if it's false, second member contains ids of revisions that were actually deleted
    std::pair<bool, std::vector<std::uint32_t>> DeletePageFull(
        std::uint32_t pageId, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions = std::unordered_set<std::uint32_t>());

    // returns whether the revision was actually deleted
    bool DeleteRevision(
        std::uint32_t revisionId, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions = std::unordered_set<std::uint32_t>());

    std::uint8_t GetIdForModelFormat(std::string model, std::string format, bool &isNew);
    std::pair<std::string, std::string> GetModelFormat(std::uint8_t id);
};