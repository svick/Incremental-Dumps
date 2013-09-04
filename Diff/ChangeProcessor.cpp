#include "ChangeProcessor.h"
#include "../Dump.h"
#include "../DumpObjects/DumpRevision.h"
#include "../Indexes/Index.h"

void ChangeProcessor::WritePage()
{
    if (currentPage != nullptr)
    {
        currentPage->Write();

        currentPage = nullptr;
    }
}

ChangeProcessor::ChangeProcessor(std::shared_ptr<WritableDump> dump)
    : dump(dump)
{}

void ChangeProcessor::Process(SiteInfoChange change)
{
    dump->siteInfo->CheckName(change.name);
    dump->siteInfo->CheckTimestamp(change.oldTimestamp);

    dump->siteInfo->timestamp = change.newTimestamp;

    dump->siteInfo->siteInfo = change.siteInfo;

    dump->siteInfo->Write();
}

void ChangeProcessor::Process(NewPageChange change)
{
    WritePage();

    DumpPage *dumpPage = new DumpPage(dump, change.page.PageId);
    dumpPage->page = change.page;

    currentPage = std::unique_ptr<DumpPage>(dumpPage);
}

void ChangeProcessor::Process(PageChange change)
{
    WritePage();

    DumpPage *dumpPage = new DumpPage(dump, change.pageChanges.PageId);

    if (HasFlag(change.flags, PageChangeFlags::NamespaceChanged))
        dumpPage->page.Namespace = change.pageChanges.Namespace;

    if (HasFlag(change.flags, PageChangeFlags::TitleChanged))
        dumpPage->page.Title =  change.pageChanges.Title;

    if (HasFlag(change.flags, PageChangeFlags::RedirectTargetChanged))
        dumpPage->page.RedirectTarget =  change.pageChanges.RedirectTarget;

    currentPage = std::unique_ptr<DumpPage>(dumpPage);
}

void ChangeProcessor::Process(NewModelFormatChange change)
{
    dump->modelFormatIndex->Add(change.id, std::make_pair(change.model, change.format));
}

void ChangeProcessor::Process(NewRevisionChange change)
{
    DumpRevision dumpRevision(dump, change.revision.RevisionId);
    dumpRevision.revision = change.revision;
    dumpRevision.SetModelFormatId(change.modelFormatId);
    dumpRevision.Write();

    currentPage->page.RevisionIds.insert(change.revision.RevisionId);
}

void ChangeProcessor::Process(RevisionChange change)
{
    DumpRevision dumpRevision(dump, change.revisionChanges.RevisionId);

    Revision &revisionChanges = change.revisionChanges;
    Revision &revision = dumpRevision.revision;

    if (HasFlag(change.flags, RevisionChangeFlags::Flags))
        revision.Flags = revisionChanges.Flags;

    if (HasFlag(change.flags, RevisionChangeFlags::ParentId))
        revision.ParentId = revisionChanges.ParentId;

    if (HasFlag(change.flags, RevisionChangeFlags::DateTime))
        revision.DateTime = revisionChanges.DateTime;

    if (HasFlag(change.flags, RevisionChangeFlags::Contributor))
        revision.Contributor = revisionChanges.Contributor;

    if (HasFlag(change.flags, RevisionChangeFlags::Comment))
        revision.Comment = revisionChanges.Comment;

    if (HasFlag(change.flags, RevisionChangeFlags::Text))
    {
        revision.Sha1 = revisionChanges.Sha1;

        if (IsPages(dump->fileHeader.Kind))
            revision.SetText(revisionChanges.GetText());
        else
            revision.TextLength = revisionChanges.TextLength;
    }

    if (HasFlag(change.flags, RevisionChangeFlags::ModelFormat))
        dumpRevision.SetModelFormatId(change.newRevisionModelFormatId);

    dumpRevision.Write();

    currentPage->page.RevisionIds.insert(revision.RevisionId);
}

void ChangeProcessor::Process(DeleteRevisionChange change)
{
    dump->DeleteRevision(change.revisionId);

    auto &revisionIds = currentPage->page.RevisionIds;
    auto deletedPos = std::find(revisionIds.begin(), revisionIds.end(), change.revisionId);
    if (deletedPos != revisionIds.end())
        revisionIds.erase(deletedPos);
}

void ChangeProcessor::Process(FullDeletePageChange change)
{
    dump->DeletePageFull(change.pageId);
}

void ChangeProcessor::Process(PartialDeletePageChange change)
{
    dump->DeletePagePartial(change.pageId);
}

void ChangeProcessor::End()
{
    WritePage();

    dump->Complete();
}