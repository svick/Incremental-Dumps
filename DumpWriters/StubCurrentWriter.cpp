#include "StubCurrentWriter.h"
#include "../DumpObjects/DumpRevision.h"

StubCurrentWriter::StubCurrentWriter(shared_ptr<WritableDump> dump)
    : dump(dump)
{}

void StubCurrentWriter::StartPage(const shared_ptr<const Page> page)
{
    this->page = unique_ptr<DumpPage>(new DumpPage(dump, page->PageId));
    this->page->page = *page;
}

void StubCurrentWriter::AddRevision(const shared_ptr<const Revision> revision)
{
    page->page.RevisionIds.push_back(revision->RevisionId);
    revisions.push_back(revision);
}

void StubCurrentWriter::EndPage()
{
    page->Write();

    for (auto revision : revisions)
    {
        DumpRevision dumpRevision(dump, false);
        dumpRevision.revision = *revision;
        dumpRevision.Write();
    }

    page = nullptr;
    revisions.clear();
}