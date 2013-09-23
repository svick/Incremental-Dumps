#pragma once

#include "../../DumpObjects/DumpObject.h"

/** @file */

/**
 * The type of change in diff dump.
 */
enum class ChangeKind : std::uint8_t
{
    SiteInfo          = 0x01,

    NewPage           = 0x10,
    ChangePage        = 0x11,
    DeletePageFull    = 0x12,
    DeletePagePartial = 0x13,

    NewRevision       = 0x20,
    ChangeRevision    = 0x21,
    DeleteRevision    = 0x22,

    NewModelFormat    = 0x30,

    TextGroup         = 0x40
};

/**
 * Represents a change that can be applied to dump.
 * This is the basic unit of diff dumps.
 */
class Change : public DumpObjectBase
{
public:
    /**
     * Writes this change to the @a stream.
     */
    void Write(std::ostream *stream);

    virtual ~Change() {}
};