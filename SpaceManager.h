#pragma once

#include <cstdint>
#include <memory>
#include <map>
#include "Index.h"

using std::int32_t;
using std::int64_t;
using std::weak_ptr;
using std::multimap;

class WritableDump;

class SpaceManager
{
private:
    weak_ptr<WritableDump> dump;
    Index<Offset, int32_t> spaceIndex;
    multimap<int32_t, Offset> spaceByLength;
public:
    SpaceManager(weak_ptr<WritableDump> dump);
    int64_t GetSpace(int32_t length);
    void Delete(int64_t offset, int32_t length);
};