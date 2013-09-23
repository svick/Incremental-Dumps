// warning: because of a cyclic dependency, "Indexes/Index.h" has to be always included before this header
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

/**
 * Used for allocating and deallocating space in the dump file.
 */
class SpaceManager
{
private:
    weak_ptr<WritableDump> dump;
    /**
     * Inverted view of ReadableDump::spaceIndex, so that blocks of free space could be searched by their size.
     */
    multimap<uint32_t, Offset> spaceByLength;
public:
    SpaceManager(weak_ptr<WritableDump> dump);

    /**
     * Allocates @a length bytes of space in the dump and returns offset to the allocated block.
     */
    uint64_t GetSpace(uint32_t length);
    /**
     * Deallocates @a length bytes starting at @a offset.
     */
    void Delete(uint64_t offset, uint32_t length);
};
