#pragma once

#include "DumpObject.h"
#include "../Dump.h"
#include "../Objects/Page.h"

using std::shared_ptr;

class DumpPage : public DumpObject
{
private:
    void Load(uint32_t pageId);
    static Page Read(shared_ptr<WritableDump> dump, Offset offset);
protected:
    virtual void WriteInternal();
    virtual void UpdateIndex(Offset offset);
public:
    Page page;

    DumpPage(weak_ptr<WritableDump> dump, uint32_t pageId);

    virtual uint32_t NewLength() const;
};