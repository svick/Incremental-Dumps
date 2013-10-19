#include <memory>
#include <string>
#include <fstream>
#include "Dump.h"
#include "Indexes/Index.h"
#include "SpaceManager.h"
#include "TextGroupsManager.h"
#include "DumpObjects/DumpRevision.h"
#include "DumpObjects/DumpPage.h"

Dump::Dump(const std::string& fileName)
    : stream(openStream(fileName))
{}

std::weak_ptr<Dump> Dump::GetSelf() const
{
    return self;
}

std::unique_ptr<std::iostream> Dump::openStream(std::string fileName)
{
    std::fstream *stream = new std::fstream(fileName, std::ios::in | std::ios::out | std::ios::binary);

    if (!stream->is_open())
    {
        errno = 0;
        // this feels dangerous, isn't there a better way?
        stream = new std::fstream(fileName, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    }

    stream->exceptions(std::ios::failbit | std::ios::badbit);

    return std::unique_ptr<std::iostream>(stream);
}

void Dump::init(std::shared_ptr<Dump> self)
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

std::shared_ptr<Dump> Dump::Create(const std::string& fileName)
{
    std::shared_ptr<Dump> dump(new Dump(fileName));
    dump->init(dump);
    return dump;
}

void Dump::Complete(DiffWriter* diffWriter)
{
    textGroupsManager->Complete(diffWriter);

    spaceIndex->Write();
    pageIdIndex->Write();
    revisionIdIndex->Write();
    textGroupIdIndex->Write();
    modelFormatIndex->Write();
}

std::pair<bool, std::vector<std::uint32_t>> Dump::DeletePage(
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

void Dump::DeletePagePartial(std::uint32_t pageId)
{
    DeletePage(pageId, false, std::unordered_set<std::uint32_t>());
}

std::pair<bool, std::vector<std::uint32_t>> Dump::DeletePageFull(
    std::uint32_t pageId, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions)
{
    return DeletePage(pageId, true, doNotDeleteRevisions);
}

bool Dump::DeleteRevision(std::uint32_t revisionId, const std::unordered_set<std::uint32_t> &doNotDeleteRevisions)
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

const std::string Dump::WikitextModel = "wikitext";
const std::string Dump::WikitextFormat = "text/x-wiki";

std::uint8_t Dump::GetIdForModelFormat(std::string model, std::string format, bool &isNew)
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

std::pair<std::string, std::string> Dump::GetModelFormat(std::uint8_t id)
{
    if (id == 0)
        return std::make_pair(WikitextModel, WikitextFormat);

    return modelFormatIndex->Get(id);
}
