// warning: because of a cyclic dependency, "Indexes/Index.h" has to be always included before this header
#pragma once

#include <cstdint>
#include <memory>
#include <map>
#include "Indexes/Index.h"

class Dump;

/**
 * Used for allocating and deallocating space in the dump file.
 */
class SpaceManager
{
private:
    std::weak_ptr<Dump> dump;
    /**
     * Inverted view of Dump::spaceIndex, so that blocks of free space could be searched by their size.
     */
    std::multimap<uint32_t, Offset> spaceByLength;
public:
    SpaceManager(std::weak_ptr<Dump> dump);

    /**
     * Allocates @a length bytes of space in the dump and returns offset to the allocated block.
     */
    uint64_t GetSpace(uint32_t length);
    /**
     * Deallocates @a length bytes starting at @a offset.
     */
    void Delete(uint64_t offset, uint32_t length);
};
