#pragma once

#include "DumpWriter.h"
#include "../DumpObjects/DumpPage.h"

class PagesHistoryWriter : public DumpWriter
{
private:
    shared_ptr<WritableDump> dump;
    unique_ptr<DumpPage> page;
    vector<shared_ptr<const Revision>> revisions;
public:
    PagesHistoryWriter(shared_ptr<WritableDump> dump);

    virtual void StartPage(const shared_ptr<const Page> page);
    virtual void AddRevision(const shared_ptr<const Revision> revision);
    virtual void EndPage();
};