#pragma once

#include "../../DumpObjects/DumpObject.h"

class ChangeVisitor;

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

    NewModelFormat    = 0x30
};

class Change : public DumpObjectBase
{
public:
    void Write(std::ostream *stream);

    virtual void Accept(ChangeVisitor &visitor) = 0;

    virtual ~Change() {}
};