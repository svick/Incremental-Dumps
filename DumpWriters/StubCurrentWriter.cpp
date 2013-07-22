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
    this->revision = revision;
}

void StubCurrentWriter::EndPage()
{
    for (uint32_t revisionId : page->page.RevisionIds)
    {
        dump->DeleteRevision(revisionId);
    }

    page->page.RevisionIds.clear();
    page->page.RevisionIds.push_back(revision->RevisionId);

    page->Write();

    DumpRevision dumpRevision(dump, false);
    dumpRevision.revision = *revision;
    dumpRevision.Write();

    page = nullptr;
    revision = nullptr;
}