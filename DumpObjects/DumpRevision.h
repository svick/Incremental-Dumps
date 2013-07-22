#pragma once

#include "DumpObject.h"
#include "../Dump.h"
#include "../Objects/Revision.h"

using std::shared_ptr;

// after DumpRevision is created, Text of its revision can't be changed
class DumpRevision : public DumpObject
{
private:
    bool withText;
    string compressedText;

    void Load(uint32_t revisionId);
    static Revision Read(shared_ptr<WritableDump> dump, Offset offset);
    void EnsureCompressed();
protected:
    virtual void WriteInternal() override;
    virtual void UpdateIndex(Offset offset, bool overwrite) override;
public:
    Revision revision;

    DumpRevision(weak_ptr<WritableDump> dump, uint32_t revisionId, bool withText);
    DumpRevision(weak_ptr<WritableDump> dump, bool withText);

    virtual uint32_t NewLength() override;
};