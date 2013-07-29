#pragma once

#include "DumpWriter.h"
#include "../DumpObjects/DumpPage.h"

class PagesHistoryWriter : public DumpWriter
{
private:
    vector<shared_ptr<const Revision>> revisions;
public:
    PagesHistoryWriter(shared_ptr<WritableDump> dump);

    virtual void AddRevision(const shared_ptr<const Revision> revision) override;
    virtual void EndPage() override;
};