#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <iostream>
#include <unordered_set>
#include "DumpObjects/FileHeader.h"
#include "DumpObjects/DumpSiteInfo.h"

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

class ReadableDump
{
protected:
    std::weak_ptr<WritableDump> self;

    ReadableDump(unique_ptr<iostream> stream);
public:
    // TODO: others should not be able to steal this stream
    unique_ptr<iostream> stream;
    unique_ptr<Index<uint32_t, Offset>> pageIdIndex;
    unique_ptr<Index<uint32_t, Offset>> revisionIdIndex;
    unique_ptr<Index<uint8_t, std::pair<std::string, std::string>>> modelFormatIndex;

    unique_ptr<DumpSiteInfo> siteInfo;

    ReadableDump(string fileName);

    std::weak_ptr<WritableDump> GetSelf() const;
};

class WritableDump : public ReadableDump
{
private:
    static unique_ptr<iostream> openStream(string fileName);

    WritableDump(string fileName);
    void init(std::weak_ptr<WritableDump> self);

public:
    static const std::string WikitextModel;
    static const std::string WikitextFormat;

    static shared_ptr<WritableDump> Create(string fileName);

    FileHeader fileHeader;
    unique_ptr<SpaceManager> spaceManager;

    // it's necessary to call this after writing is finished
    void WriteIndexes();

    // also recursively deletes revisions of the given page
    void DeletePage(std::uint32_t pageId, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions = std::unordered_set<std::uint32_t>());
    void DeleteRevision(std::uint32_t revisionId, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions = std::unordered_set<std::uint32_t>());

    std::uint8_t GetIdForModelFormat(std::string model, std::string format, bool &isNew);
    std::pair<std::string, std::string> GetModelFormat(std::uint8_t id);
};