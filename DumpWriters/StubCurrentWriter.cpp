#include "StubCurrentWriter.h"
#include "../DumpObjects/DumpRevision.h"

StubCurrentWriter::StubCurrentWriter(shared_ptr<WritableDump> dump)
    : DumpWriter(dump)
{}

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