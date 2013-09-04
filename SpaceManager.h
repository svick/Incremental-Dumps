// warning, because of a cyclic dependency, "Indexes/Index.h" has to be always included before this header
#pragma once

#include <cstdint>
#include <memory>
#include <map>
#include "Indexes/Index.h"

using std::uint32_t;
using std::uint64_t;
using std::weak_ptr;
using std::multimap;

class WritableDump;

class SpaceManager
{
private:
    weak_ptr<WritableDump> dump;
    multimap<uint32_t, Offset> spaceByLength;
public:

    SpaceManager(weak_ptr<WritableDump> dump);

    uint64_t GetSpace(uint32_t length);
    void Delete(uint64_t offset, uint32_t length);
};
