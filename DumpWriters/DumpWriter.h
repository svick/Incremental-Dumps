#pragma once

#include "IDumpWriter.h"
#include "../Dump.h"
#include "../DumpObjects/DumpPage.h"
#include "../Diff/DiffWriter.h"

class DumpWriter : public IDumpWriter
{
private:
    std::shared_ptr<WritableDump> dump;
    std::unique_ptr<DiffWriter> diffWriter;

    std::unique_ptr<DumpPage> page;
    // this is necessary, so that page object can be on disk before its revision objects
    // it shouldn't waste too much memory, because these revisions don't contain any text
    std::vector<std::shared_ptr<const Revision>> revisions;
    Page oldPage;

    std::vector<bool> unvisitedPageIds;
    // list of revision ids that were added to some page (not necessarily completely new in the dump)
    std::unordered_set<std::uint32_t> newRevisionIds;
    bool withText;

public:
    DumpWriter(std::shared_ptr<WritableDump> dump, bool withText, std::unique_ptr<DiffWriter> diffWriter = nullptr);

    virtual void StartPage(const std::shared_ptr<const Page> page) override;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) override;
    virtual void EndPage() override;
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) override;
    virtual void SetDumpKind(DumpKind dumpKind) override;
    virtual void EndDump() override;
};