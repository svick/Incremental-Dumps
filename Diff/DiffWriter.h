#pragma once

#include <string>
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

    void EnsurePageWritten();
public:
    DiffWriter(std::string fileName);

    void SetSiteInfo(const SiteInfo &siteInfo, DumpKind dumpKind);

    void StartNewPage(const Page &page);
    void StartExistingPage(const Page &oldPage, const Page &newPage);
    // if it's known that the page itself didn't change
    void StartExistingPage(std::uint32_t pageId);
    
    void NewModelFormat(std::uint8_t id, const std::string &model, const std::string &format);

    void NewRevision(const Revision &revision, std::uint8_t modelFormatId);
    void ChangeRevision(const Revision &oldRevision, Revision &newRevision, std::uint8_t newModelFormatId);
    void DeleteRevision(std::uint32_t revisionId);

    void EndPage();

    void DeletePage(const std::uint32_t pageId);
};