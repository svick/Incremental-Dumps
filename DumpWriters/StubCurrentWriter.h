#pragma once

#include "DumpWriter.h"
#include "../DumpObjects/DumpPage.h"

class StubCurrentWriter : public DumpWriter
{
private:
    shared_ptr<WritableDump> dump;
    unique_ptr<DumpPage> page;
    shared_ptr<const Revision> revision;
public:
    StubCurrentWriter(shared_ptr<WritableDump> dump);

    virtual void StartPage(const shared_ptr<const Page> page);
    virtual void AddRevision(const shared_ptr<const Revision> revision);
    virtual void EndPage();
};