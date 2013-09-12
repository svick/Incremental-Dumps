#pragma once

#include <string>
#include <queue>
#include "Changes/PageChange.h"
#include "../Objects/Page.h"
#include "../Objects/Revision.h"
#include "../Objects/SiteInfo.h"
#include "../DumpKind.h"

class DiffWriter
{
private:
    std::unique_ptr<std::ostream> stream;
    DumpKind dumpKind;

    bool dumpStarted;
    bool pageStarted;
    bool pageWritten;
    std::unique_ptr<PageChange> unwrittenPage;

    const std::string name;
    const std::string oldTimestamp;
    const std::string newTimestamp;

    std::queue<std::unique_ptr<Change>> changeQueue;

    void EnsurePageWritten();

    // either saves the change directly, or adds it to queue
    template <typename TChange>
    void Process(TChange &change);

    void HandleQueue();
public:
    DiffWriter(const std::string &fileName, const std::string &name, const std::string &oldTimestamp, const std::string &newTimestamp);

    void SetSiteInfo(const SiteInfo &siteInfo, DumpKind dumpKind);

    void StartNewPage(const Page &page);
    void StartExistingPage(const Page &oldPage, const Page &newPage);
    // if it's known that the page itself didn't change
    void StartExistingPage(std::uint32_t pageId);
    
    void NewModelFormat(std::uint8_t id, const std::string &model, const std::string &format);

    void NewRevision(const Revision &revision, std::uint8_t modelFormatId, std::uint8_t textId);
    void ChangeRevision(const Revision &oldRevision, Revision &newRevision, std::uint8_t newModelFormatId, std::uint8_t textId);
    void DeleteRevision(std::uint32_t revisionId);

    void EndPage();

    void DeletePageFull(std::uint32_t pageId);
    void DeletePagePartial(std::uint32_t pageId);

    void SetTextGroup(const std::string& compressedTexts);

    void Complete();
};