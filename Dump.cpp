#include <memory>
#include <string>
#include <fstream>
#include "Dump.h"
#include "Indexes/Index.h"
#include "SpaceManager.h"
#include "TextGroupsManager.h"
#include "DumpObjects/DumpRevision.h"
#include "DumpObjects/DumpPage.h"

using std::move;
using std::unique_ptr;
using std::make_shared;
using std::string;
using std::fstream;
using std::ios;

ReadableDump::ReadableDump(unique_ptr<iostream> stream)
    : stream(move(stream))
{}

ReadableDump::ReadableDump(string fileName)
    : stream(unique_ptr<fstream>(new fstream(fileName, ios::in | ios::binary)))
{}

weak_ptr<WritableDump> ReadableDump::GetSelf() const
{
    return self;
}

unique_ptr<iostream> WritableDump::openStream(string fileName)
{
    fstream *stream = new fstream(fileName, ios::in | ios::out | ios::binary);

    if (!stream->is_open())
    {
        // this feels dangerous, isn't there a better way?
        stream = new fstream(fileName, ios::in | ios::out | ios::binary | ios::trunc);
    }

    stream->exceptions(ios::failbit | ios::badbit);

    return unique_ptr<iostream>(stream);
}

WritableDump::WritableDump(string fileName)
    : ReadableDump(openStream(fileName))
{}

void WritableDump::init(std::shared_ptr<WritableDump> self)
{
    this->self = self;

    if (stream->peek() == EOF)
    {
        stream->clear();
        fileHeader = FileHeader(self);
        fileHeader.Write();
        isNew = true;
    }
    else
    {
        fileHeader = FileHeader::Read(*this);
        isNew = false;
    }

    spaceIndex = std::unique_ptr<Index<Offset, std::uint32_t>>(
        new Index<Offset, std::uint32_t>(self, &fileHeader.FreeSpaceIndexRoot, true));

    spaceManager = std::unique_ptr<SpaceManager>(new SpaceManager(self));

    pageIdIndex = std::unique_ptr<Index<std::uint32_t, Offset>>(
        new Index<std::uint32_t, Offset>(self, &fileHeader.PageIdIndexRoot));
    revisionIdIndex = std::unique_ptr<Index<std::uint32_t, Offset>>(
        new Index<std::uint32_t, Offset>(self, &fileHeader.RevisionIdIndexRoot));
    textGroupIdIndex = std::unique_ptr<Index<std::uint32_t, Offset>>(
        new Index<std::uint32_t, Offset>(self, &fileHeader.TextGroupIdIndexRoot));
    modelFormatIndex = std::unique_ptr<Index<std::uint8_t, std::pair<std::string, std::string>>>(
        new Index<std::uint8_t, std::pair<std::string, std::string>>(self, &fileHeader.ModelFormatIndexRoot));

    textGroupsManager = std::unique_ptr<TextGroupsManager>(new TextGroupsManager(self));

    siteInfo = std::unique_ptr<DumpSiteInfo>(new DumpSiteInfo(self));
}

shared_ptr<WritableDump> WritableDump::Create(string fileName)
{
    shared_ptr<WritableDump> dump(new WritableDump(fileName));
    dump->init(dump);
    return dump;
}

void WritableDump::Complete(DiffWriter* diffWriter)
{
    textGroupsManager->Complete(diffWriter);

    spaceIndex->Write();
    pageIdIndex->Write();
    revisionIdIndex->Write();
    textGroupIdIndex->Write();
    modelFormatIndex->Write();
}

std::pair<bool, std::vector<std::uint32_t>> WritableDump::DeletePage(
    std::uint32_t pageId, bool full, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions)
{
    Offset offset = pageIdIndex->Get(pageId);
    DumpPage page(self, pageId);
    std::uint32_t length = page.NewLength();

    bool allRevisionsDeleted = true;
    std::vector<std::uint32_t> deletedRevisions;

    if (full)
    {
        for (auto revisionId : page.page.RevisionIds)
        {
            if (DeleteRevision(revisionId, doNotDeleteRevisions))
                deletedRevisions.push_back(revisionId);
            else
                allRevisionsDeleted = false;
        }
    }

    pageIdIndex->Remove(pageId);
    spaceManager->Delete(offset.value, length);

    if (allRevisionsDeleted)
        return std::make_pair(true, std::vector<std::uint32_t>());
    
    return std::make_pair(false, deletedRevisions);
}

void WritableDump::DeletePagePartial(std::uint32_t pageId)
{
    DeletePage(pageId, false, std::unordered_set<std::uint32_t>());
}

std::pair<bool, std::vector<std::uint32_t>> WritableDump::DeletePageFull(
    std::uint32_t pageId, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions)
{
    return DeletePage(pageId, true, doNotDeleteRevisions);
}

bool WritableDump::DeleteRevision(std::uint32_t revisionId, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions)
{
    if (doNotDeleteRevisions.count(revisionId) != 0)
        return false;

    Offset offset = revisionIdIndex->Get(revisionId);
    DumpRevision revision(self, revisionId);
    std::uint32_t length = revision.NewLength();

    revision.DeleteText();

    revisionIdIndex->Remove(revisionId);
    spaceManager->Delete(offset.value, length);

    return true;
}

const std::string WritableDump::WikitextModel = "wikitext";
const std::string WritableDump::WikitextFormat = "text/x-wiki";

std::uint8_t WritableDump::GetIdForModelFormat(std::string model, std::string format, bool &isNew)
{
    isNew = false;

    if (model == WikitextModel && format == WikitextFormat)
        return 0;

    auto searchedPair = make_pair(model, format);
    
    auto found = std::find_if(
        modelFormatIndex->begin(),
        modelFormatIndex->end(),
        [&](std::pair<std::uint8_t, std::pair<std::string, std::string>> foundItem){ return foundItem.second == searchedPair; });

    if (found != modelFormatIndex->end())
        return found->first;

    std::uint8_t newId = getNewId(*modelFormatIndex);

    modelFormatIndex->Add(newId, searchedPair);
    isNew = true;

    return newId;
}

std::pair<std::string, std::string> WritableDump::GetModelFormat(std::uint8_t id)
{
    if (id == 0)
        return std::make_pair(WikitextModel, WikitextFormat);

    return modelFormatIndex->Get(id);
}