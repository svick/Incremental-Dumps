#pragma once

#include "DumpWriter.h"
#include "../DumpObjects/DumpPage.h"

class StubCurrentWriter : public DumpWriter
{
private:
    shared_ptr<const Revision> revision;
public:
    StubCurrentWriter(shared_ptr<WritableDump> dump);

    virtual void AddRevision(const shared_ptr<const Revision> revision) override;
    virtual void EndPage() override;
};