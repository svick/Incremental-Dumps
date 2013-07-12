#pragma once

#include "DumpObject.h"
#include "../Dump.h"
#include "../Objects/Revision.h"

using std::shared_ptr;

class DumpRevision : public DumpObject
{
private:
    bool withText;

    void Load(uint32_t revisionId);
    static Revision Read(shared_ptr<WritableDump> dump, Offset offset);
protected:
    virtual void WriteInternal() override;
    virtual void UpdateIndex(Offset offset, bool overwrite) override;
public:
    Revision revision;

    DumpRevision(weak_ptr<WritableDump> dump, uint32_t revisionId, bool withText);
    DumpRevision(weak_ptr<WritableDump> dump, bool withText);

    virtual uint32_t NewLength() const override;
};