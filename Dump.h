#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <iostream>
#include "DumpObjects/FileHeader.h"

using std::int32_t;
using std::int64_t;
using std::unique_ptr;
using std::shared_ptr;
using std::string;
using std::iostream;

class WritableDump;

template<typename TKey, typename TValue>
class Index;

class SpaceManager;

class ReadableDump
{
protected:
    weak_ptr<WritableDump> self;

    ReadableDump(unique_ptr<iostream> stream);
public:
    // TODO: others should not be able to steal this stream
    unique_ptr<iostream> stream;
    unique_ptr<Index<uint32_t, Offset>> pageIdIndex;
    unique_ptr<Index<uint32_t, Offset>> revisionIdIndex;

    ReadableDump(string fileName);

    weak_ptr<WritableDump> GetSelf() const;
};

class WritableDump : public ReadableDump
{
private:
    static unique_ptr<iostream> openStream(string fileName);

    WritableDump(string fileName);
    void init(weak_ptr<WritableDump> self);
public:
    static shared_ptr<WritableDump> Create(string fileName);

    FileHeader fileHeader;
    unique_ptr<SpaceManager> spaceManager;

    // it's necessary to call this after writing is finished
    void WriteIndexes();

    void DeleteRevision(uint32_t revisionId);
};
