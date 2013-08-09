#include <memory>
#include <string>
#include <fstream>
#include "Dump.h"
#include "Indexes/Index.h"
#include "SpaceManager.h"
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

void WritableDump::init(weak_ptr<WritableDump> self)
{
    this->self = self;

    if (stream->peek() == EOF)
    {
        stream->clear();
        fileHeader = FileHeader(self);
        fileHeader.Write();
    }
    else
    {
        fileHeader = FileHeader::Read(*this);
    }

    spaceManager = unique_ptr<SpaceManager>(new SpaceManager(self));

    pageIdIndex = unique_ptr<Index<uint32_t, Offset>>(
        new Index<uint32_t, Offset>(self, shared_ptr<Offset>(self.lock(), &fileHeader.PageIdIndexRoot)));
    revisionIdIndex = unique_ptr<Index<uint32_t, Offset>>(
        new Index<uint32_t, Offset>(self, shared_ptr<Offset>(self.lock(), &fileHeader.RevisionIdIndexRoot)));
    modelFormatIndex = unique_ptr<Index<uint8_t, std::pair<std::string, std::string>>>(
        new Index<uint8_t, std::pair<std::string, std::string>>(self, shared_ptr<Offset>(self.lock(), &fileHeader.ModelFormatIndexRoot)));

    siteInfo = unique_ptr<DumpSiteInfo>(new DumpSiteInfo(self));
}

shared_ptr<WritableDump> WritableDump::Create(string fileName)
{
    shared_ptr<WritableDump> dump(new WritableDump(fileName));
    dump->init(dump);
    return dump;
}

void WritableDump::WriteIndexes()
{
    spaceManager->spaceIndex.Write();

    pageIdIndex->Write();
    revisionIdIndex->Write();
    modelFormatIndex->Write();
}

void WritableDump::DeletePage(std::uint32_t pageId)
{
    Offset offset = pageIdIndex->Get(pageId);
    DumpPage page(self, pageId);
    std::uint32_t length = page.NewLength();

    for (auto revisionId : page.page.RevisionIds)
    {
        DeleteRevision(revisionId);
    }

    pageIdIndex->Remove(pageId);
    spaceManager->Delete(offset.value, length);
}

void WritableDump::DeleteRevision(std::uint32_t revisionId)
{
    Offset offset = revisionIdIndex->Get(revisionId);
    DumpRevision revision(self, revisionId, false);
    std::uint32_t length = revision.NewLength();

    revisionIdIndex->Remove(revisionId);
    spaceManager->Delete(offset.value, length);
}

const std::string WritableDump::WikitextModel = "wikitext";
const std::string WritableDump::WikitextFormat = "text/x-wiki";

std::uint8_t WritableDump::GetIdForModelFormat(std::string model, std::string format)
{
    if (model == WikitextModel && format == WikitextFormat)
        return 0;

    auto searchedPair = make_pair(model, format);
    
    auto found = std::find_if(
        modelFormatIndex->begin(),
        modelFormatIndex->end(),
        [&](std::pair<std::uint8_t, std::pair<std::string, std::string>> foundItem){ return foundItem.second == searchedPair; });

    if (found != modelFormatIndex->end())
        return found->first;

    // pair's default ordering will work fine here
    auto maxPairRef = std::max_element(modelFormatIndex->begin(), modelFormatIndex->end());
    uint8_t newId;
    if (maxPairRef == modelFormatIndex->end())
        newId = 1;
    else
        newId = maxPairRef->first + 1;

    modelFormatIndex->Add(newId, searchedPair);
    return newId;
}

std::pair<std::string, std::string> WritableDump::GetModelFormat(std::uint8_t id)
{
    if (id == 0)
        return std::make_pair(WikitextModel, WikitextFormat);

    return modelFormatIndex->Get(id);
}