#include <memory>
#include <string>
#include <fstream>
#include "Dump.h"
#include "SpaceManager.h"

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
}
