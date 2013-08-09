#pragma once

#include "DumpObject.h"
#include "../Dump.h"
#include "../Objects/Page.h"

using std::shared_ptr;

class DumpPage : public DumpObject
{
private:
    Page originalPage;
    bool wasLoaded;

    void Load(uint32_t pageId);
    static Page Read(shared_ptr<WritableDump> dump, Offset offset);
protected:
    virtual void WriteInternal() override;
    virtual void UpdateIndex(Offset offset, bool overwrite) override;
public:
    Page page;

    DumpPage(weak_ptr<WritableDump> dump, uint32_t pageId);
    DumpPage(weak_ptr<WritableDump> dump, Offset offset);

    virtual void Write() override;
    virtual std::uint32_t NewLength() override;
};