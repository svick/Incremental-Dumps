#pragma once

#include "DumpWriter.h"
#include "../DumpObjects/DumpPage.h"

class StubHistoryWriter : public DumpWriter
{
private:
    vector<shared_ptr<const Revision>> revisions;
public:
    StubHistoryWriter(shared_ptr<WritableDump> dump);

    virtual void AddRevision(const shared_ptr<const Revision> revision) override;
    virtual void EndPage() override;
};