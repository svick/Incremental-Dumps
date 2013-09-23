#pragma once

#include "IDumpWriter.h"
#include "../Dump.h"
#include "../DumpObjects/DumpPage.h"
#include "../Diff/DiffWriter.h"

/**
 * The IDumpWriter that actually writes to a dump (and is not just a wrapper or composite).
 */
class DumpWriter : public IDumpWriter
{
private:
    std::shared_ptr<WritableDump> dump;
    std::unique_ptr<DiffWriter> diffWriter;

    std::unique_ptr<DumpPage> page;
    Page oldPage;

    std::vector<bool> unvisitedPageIds;

    /**
     * List of revision ids that were newly added to some page (but not necessarily completely new in the dump)
     */
    std::unordered_set<std::uint32_t> newRevisionIds;
    bool withText;

    /**
     * Removes namespace from Page::Title of the given page.
     */
    void RemoveNamespace(Page& page);

    /**
     * Makes sure the Revision::Comment of the given revision fits into 255 characters.
     */
    void NormalizeComment(Revision& revision);
public:
    /**
     * @param dump The dump to update.
     * @param withText Whether the dump contains texts of revisions.
     * @param diffWriter Records all actions done by this writer into a diff dump.
     */
    DumpWriter(std::shared_ptr<WritableDump> dump, bool withText, std::unique_ptr<DiffWriter> diffWriter = nullptr);

    virtual void StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace) OVERRIDE;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) OVERRIDE;
    virtual void EndPage() OVERRIDE;
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) OVERRIDE;
    virtual void SetDumpKind(DumpKind dumpKind) OVERRIDE;
    virtual void Complete() OVERRIDE;
};