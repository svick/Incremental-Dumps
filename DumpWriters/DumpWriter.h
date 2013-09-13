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
    Page oldPage;

    std::vector<bool> unvisitedPageIds;
    // list of revision ids that were added to some page (not necessarily completely new in the dump)
    std::unordered_set<std::uint32_t> newRevisionIds;
    bool withText;

    void RemoveNamespace(Page& page);
    void NormalizeComment(Revision& revision);
public:
    DumpWriter(std::shared_ptr<WritableDump> dump, bool withText, std::unique_ptr<DiffWriter> diffWriter = nullptr);

    virtual void StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace) OVERRIDE;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) OVERRIDE;
    virtual void EndPage() OVERRIDE;
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) OVERRIDE;
    virtual void SetDumpKind(DumpKind dumpKind) OVERRIDE;
    virtual void Complete() OVERRIDE;
};