#include "DumpWriter.h"
#include "../DumpObjects/DumpRevision.h"
#include <algorithm>

void DumpWriter::SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo)
{
    dump->siteInfo->siteInfo = *siteInfo;
    dump->siteInfo->Write();
}

void DumpWriter::StartPage(const shared_ptr<const Page> page)
{
    this->page = unique_ptr<DumpPage>(new DumpPage(dump, page->PageId));
    this->page->page = *page;
}

const std::vector<std::uint32_t> DumpWriter::GetRevisionIds() const
{
    return this->page->page.RevisionIds;
}

void DumpWriter::AddRevision(const shared_ptr<const Revision> revision)
{
    page->page.RevisionIds.push_back(revision->RevisionId);
    revisions.push_back(revision);
}

void DumpWriter::DeleteRevision(std::uint32_t revisionId)
{
    dump->DeleteRevision(revisionId);

    auto &revisionIds = page->page.RevisionIds;
    auto toDeleteIt = std::find(revisionIds.begin(), revisionIds.end(), revisionId);
    if (toDeleteIt == revisionIds.end())
        throw DumpException();
    
    revisionIds.erase(toDeleteIt);
}

void DumpWriter::EndPage()
{
    page->Write();

    for (auto revision : revisions)
    {
        DumpRevision dumpRevision(dump, withText);
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

void DumpWriter::WriteIndexes()
{
    dump->WriteIndexes();
}
