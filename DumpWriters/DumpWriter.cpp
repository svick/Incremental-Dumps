#include <algorithm>
#include "DumpWriter.h"
#include "../DumpObjects/DumpRevision.h"
#include "../CollectionHelpers.h"
#include "../Indexes/Index.h"
#include "../format.h"

void DumpWriter::RemoveNamespace(Page& page)
{
    std::string namespapceName = dump->siteInfo->siteInfo.Namespaces.at(page.Namespace).second;

    if (namespapceName.empty())
        return;

    namespapceName.append(":");

    if (page.Title.substr(0, namespapceName.length()) != namespapceName)
        throw DumpException();

    page.Title.erase(0, namespapceName.length());
}

void DumpWriter::NormalizeComment(Revision& revision)
{
    std::string &comment = revision.Comment;
    if (comment.length() > 255)
    {
        // invalid UTF-8 at the end of a string is represented as U+FFFD
        // this can get string over 255 bytes, so that character needs to be removed

        size_t charsToErase;

        std::string replacementChar = "\xEF\xBF\xBD"; // UTF-8 encoded U+FFFD REPLACEMENT CHARACTER
        if (comment.substr(comment.length() - 3) == replacementChar)
            charsToErase = 3;
        else if (comment.substr(comment.length() - 6) == replacementChar + "...")
            charsToErase = 1;
        else
            throw DumpException();

        comment.erase(comment.length() - charsToErase, charsToErase);
    }
}

DumpWriter::DumpWriter(std::shared_ptr<Dump> dump, bool withText, std::unique_ptr<DiffWriter> diffWriter)
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

void DumpWriter::StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace)
{
    std::uint32_t pageId = page->PageId;

    this->page = std::unique_ptr<DumpPage>(new DumpPage(dump, pageId));
    oldPage = this->page->page;
    this->page->page = *page;

    if (titleWithNamespace)
        RemoveNamespace(this->page->page);

    unset(unvisitedPageIds, pageId);

    if (diffWriter != nullptr)
        this->page->WriteDiff(*diffWriter);
}

void DumpWriter::AddRevision(const std::shared_ptr<const Revision> revision)
{
    page->page.RevisionIds.insert(revision->RevisionId);

    DumpRevision dumpRevision(dump, revision->RevisionId);
    dumpRevision.revision = *revision;

    NormalizeComment(dumpRevision.revision);

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

void DumpWriter::EndPage()
{
    page->Write();

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
}

void DumpWriter::SetDumpKind(DumpKind dumpKind)
{
    if (withText)
        dumpKind |= DumpKind::Pages;

    if (dump->isNew)
    {
        dump->fileHeader.Kind = dumpKind;
        dump->fileHeader.Write();
    }
    else
    {
        if (dump->fileHeader.Kind != dumpKind)
            throw UserException(str(fmt::Format(
                "The specified dump kind ({0}) is not the same as the kind of the dump ({1}).") << dumpKind << dump->fileHeader.Kind));
    }
}

void DumpWriter::Complete()
{
    for (std::uint32_t i = 0; i < unvisitedPageIds.size(); i++)
    {
        if (unvisitedPageIds.at(i))
        {
            auto deletedRevisions = dump->DeletePageFull(i, newRevisionIds);

            if (diffWriter != nullptr)
            {
                if (deletedRevisions.first)
                {
                    diffWriter->DeletePageFull(i);
                }
                else
                {
                    diffWriter->DeletePagePartial(i);

                    for (auto revisionId : deletedRevisions.second)
                        diffWriter->DeleteRevision(revisionId);
                }
            }
        }
    }

    dump->Complete(diffWriter.get());

    if (diffWriter != nullptr)
        diffWriter->Complete();
}
