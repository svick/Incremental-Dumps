#include "DumpWriter.h"
#include "../DumpObjects/DumpRevision.h"
#include "../CollectionHelpers.h"
#include "../Indexes/Index.h"
#include <algorithm>

DumpWriter::DumpWriter(std::shared_ptr<WritableDump> dump, bool withText, std::unique_ptr<DiffWriter> diffWriter)
    : dump(dump), withText(withText), diffWriter(std::move(diffWriter))
{
    for (auto pair : *dump->pageIdIndex)
    {
        std::uint32_t pageId = pair.first;
        set(unvisitedPageIds, pageId);
    }
}

void DumpWriter::SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo)
{
    dump->siteInfo->siteInfo = *siteInfo;
    dump->siteInfo->Write();

    if (diffWriter != nullptr)
        diffWriter->SetSiteInfo(*siteInfo, dump->fileHeader.Kind);
}

void DumpWriter::StartPage(const std::shared_ptr<const Page> page)
{
    std::uint32_t pageId = page->PageId;
    this->page = std::unique_ptr<DumpPage>(new DumpPage(dump, pageId));
    oldPage = this->page->page;
    this->page->page = *page;
    unset(unvisitedPageIds, pageId);
}

void DumpWriter::AddRevision(const std::shared_ptr<const Revision> revision)
{
    page->page.RevisionIds.push_back(revision->RevisionId);
    revisions.push_back(revision);
}

void DumpWriter::EndPage()
{
    page->Write(diffWriter.get());

    for (auto revision : revisions)
    {
        DumpRevision dumpRevision(dump, revision->RevisionId, false);
        dumpRevision.revision = *revision;

        if (diffWriter != nullptr)
        {
            bool isNew;
            std::uint8_t modelFormatId = dumpRevision.GetModelFormatId(isNew);

            if (isNew)
                diffWriter->NewModelFormat(modelFormatId, dumpRevision.revision.Model, dumpRevision.revision.Format);
        }

        bool newRevision = !contains(oldPage.RevisionIds, revision->RevisionId);

        if (newRevision)
            newRevisionIds.insert(revision->RevisionId);

        dumpRevision.Write(diffWriter.get(), newRevision);
    }

    auto deletedRevisionIds = except(oldPage.RevisionIds, page->page.RevisionIds);

    for (auto revisionId : deletedRevisionIds)
    {
        dump->DeleteRevision(revisionId, newRevisionIds);

        if (diffWriter != nullptr)
            diffWriter->DeleteRevision(revisionId);
    }

    if (diffWriter != nullptr)
        diffWriter->EndPage();
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
        {
            dump->DeletePage(i, newRevisionIds);

            if (diffWriter != nullptr)
                diffWriter->DeletePage(i);
        }
    }

    dump->WriteIndexes();
}