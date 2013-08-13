#include "DumpWriter.h"
#include "../DumpObjects/DumpRevision.h"
#include "../CollectionHelpers.h"
#include "../Indexes/Index.h"
#include <algorithm>

DumpWriter::DumpWriter(std::shared_ptr<WritableDump> dump, bool withText)
    : dump(dump), withText(withText)
{
    for (auto pair : *dump->pageIdIndex)
    {
        std::uint32_t pageId = pair.first;
        if (unvisitedPageIds.size() <= pageId)
            unvisitedPageIds.resize(pageId + 1);
        unvisitedPageIds.at(pageId) = true;
    }
}

void DumpWriter::SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo)
{
    dump->siteInfo->siteInfo = *siteInfo;
    dump->siteInfo->Write();
}

void DumpWriter::StartPage(const std::shared_ptr<const Page> page)
{
    std::uint32_t pageId = page->PageId;
    this->page = std::unique_ptr<DumpPage>(new DumpPage(dump, pageId));
    oldRevisionIds = this->page->page.RevisionIds;
    this->page->page = *page;
    if (pageId < unvisitedPageIds.size())
        unvisitedPageIds.at(pageId) = false;
}

void DumpWriter::AddRevision(const std::shared_ptr<const Revision> revision)
{
    page->page.RevisionIds.push_back(revision->RevisionId);
    revisions.push_back(revision);
}

void DumpWriter::EndPage()
{
    page->Write();

    auto deletedRevisionIds = except(oldRevisionIds, page->page.RevisionIds);

    for (auto revisionId : deletedRevisionIds)
        dump->DeleteRevision(revisionId);

    for (auto revision : revisions)
    {
        DumpRevision dumpRevision(dump, revision->RevisionId, false);
        dumpRevision.revision = *revision;
        dumpRevision.Write();
    }

    page = nullptr;
    revisions.clear();
}

void DumpWriter::SetDumpKind(DumpKind dumpKind)
{
    if (withText)
        dumpKind |= DumpKind::Pages;

    dump->fileHeader.Kind = dumpKind;
    dump->fileHeader.Write();
}

void DumpWriter::EndDump()
{
    for (std::uint32_t i = 0; i < unvisitedPageIds.size(); i++)
    {
        if (unvisitedPageIds.at(i))
            dump->DeletePage(i);
    }

    dump->WriteIndexes();
}
