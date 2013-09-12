#include <fstream>
#include "DiffWriter.h"
#include "DiffReader.h"
#include "Changes/SiteInfoChange.h"
#include "Changes/NewPageChange.h"
#include "Changes/NewModelFormatChange.h"
#include "Changes/NewRevisionChange.h"
#include "Changes/RevisionChange.h"
#include "Changes/DeleteRevisionChange.h"
#include "Changes/FullDeletePageChange.h"
#include "Changes/PartialDeletePageChange.h"
#include "../DumpObjects/FileHeader.h"

void DiffWriter::EnsurePageWritten()
{
    if (!pageStarted)
        throw DumpException();

    if (!pageWritten)
    {
        Process(*unwrittenPage);
        unwrittenPage = nullptr;
        pageWritten = true;
    }
}

template <typename TChange>
void DiffWriter::Process(TChange &change)
{
    if (IsPages(dumpKind))
        changeQueue.push(std::unique_ptr<Change>(new TChange(change)));
    else
        change.Write(stream.get());
}

void DiffWriter::HandleQueue()
{
    while (!changeQueue.empty())
    {
        changeQueue.front()->Write(stream.get());
        changeQueue.pop();
    }
}

DiffWriter::DiffWriter(const std::string &fileName, const std::string &name, const std::string &oldTimestamp, const std::string &newTimestamp)
    : stream(std::unique_ptr<std::ostream>(new std::ofstream(fileName, std::ios::binary))),
        dumpStarted(false), pageStarted(false), pageWritten(false),
        name(name), oldTimestamp(oldTimestamp), newTimestamp(newTimestamp)
{}

void DiffWriter::SetSiteInfo(const SiteInfo &siteInfo, DumpKind dumpKind)
{
    if (dumpStarted)
        throw DumpException();

    this->dumpKind = dumpKind;

    stream->write(Diff::MagicNumber.data(), Diff::MagicNumber.length());
    DumpObject::WriteValue(*stream, FileHeader::FileFormatVersion);
    DumpObject::WriteValue(*stream, FileHeader::FileDataVersion);
    DumpObject::WriteValue(*stream, dumpKind);

    dumpStarted = true;

    SiteInfoChange change(siteInfo, name, oldTimestamp, newTimestamp);
    change.Write(stream.get());
}

void DiffWriter::StartNewPage(const Page &page)
{
    if (!dumpStarted)
        throw DumpException();
    if (pageStarted)
        throw DumpException();

    NewPageChange change(page);
    Process(change);

    pageWritten = true;
    pageStarted = true;
}

void DiffWriter::StartExistingPage(const Page &oldPage, const Page &newPage)
{
    if (!dumpStarted)
        throw DumpException();
    if (pageStarted)
        throw DumpException();

    PageChange change(oldPage, newPage);

    if (change.HasChanges())
    {
        Process(change);
        pageWritten = true;
    }
    else
    {
        unwrittenPage = std::unique_ptr<PageChange>(new PageChange(change));
        pageWritten = false;
    }

    pageStarted = true;
}

void DiffWriter::StartExistingPage(std::uint32_t pageId)
{
    if (!dumpStarted)
        throw DumpException();
    if (pageStarted)
        throw DumpException();

    unwrittenPage = std::unique_ptr<PageChange>(new PageChange(pageId));
    pageWritten = false;

    pageStarted = true;
}

void DiffWriter::NewModelFormat(std::uint8_t id, const std::string &model, const std::string &format)
{
    if (!dumpStarted)
        throw DumpException();
    if (id == 0)
        throw DumpException();

    NewModelFormatChange change(id, model, format);
    Process(change);
}

void DiffWriter::NewRevision(const Revision &revision, std::uint8_t modelFormatId, std::uint8_t textId)
{
    if (!dumpStarted)
        throw DumpException();
    if (!pageStarted)
        throw DumpException();

    EnsurePageWritten();

    NewRevisionChange change(revision, modelFormatId, IsPages(dumpKind), textId);
    Process(change);
}

void DiffWriter::ChangeRevision(
    const Revision &oldRevision, Revision &newRevision, std::uint8_t newModelFormatId , std::uint8_t textId)
{
    if (!dumpStarted)
        throw DumpException();
    if (!pageStarted)
        throw DumpException();

    EnsurePageWritten();

    RevisionChange change(oldRevision, newRevision, newModelFormatId, IsPages(dumpKind), textId);
    Process(change);
}

void DiffWriter::DeleteRevision(std::uint32_t revisionId)
{
    if (!dumpStarted)
        throw DumpException();

    if (pageStarted)
        EnsurePageWritten();

    DeleteRevisionChange change(revisionId);
    Process(change);
}

void DiffWriter::EndPage()
{
    if (!dumpStarted)
        throw DumpException();
    if (!pageStarted)
        throw DumpException();

    if (!pageWritten)
        unwrittenPage = nullptr;

    pageStarted = false;
}

void DiffWriter::DeletePageFull(std::uint32_t pageId)
{
    if (!dumpStarted)
        throw DumpException();
    if (pageStarted)
        throw DumpException();

    FullDeletePageChange change(pageId);
    Process(change);
}

void DiffWriter::DeletePagePartial(std::uint32_t pageId)
{
    if (!dumpStarted)
        throw DumpException();
    if (pageStarted)
        throw DumpException();

    PartialDeletePageChange change(pageId);
    Process(change);
}

void DiffWriter::SetTextGroup(const std::string& compressedTexts)
{
    if (!dumpStarted)
        throw DumpException();

    DiffTextGroup diffTextGroup(compressedTexts);
    diffTextGroup.Write(stream.get());

    HandleQueue();
}

void DiffWriter::Complete()
{
    HandleQueue();
}