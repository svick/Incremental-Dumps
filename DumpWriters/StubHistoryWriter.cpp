#include "StubHistoryWriter.h"
#include "../DumpObjects/DumpRevision.h"

StubHistoryWriter::StubHistoryWriter(shared_ptr<WritableDump> dump)
    : DumpWriter(dump)
{}

void StubHistoryWriter::AddRevision(const shared_ptr<const Revision> revision)
{
    page->page.RevisionIds.push_back(revision->RevisionId);
    revisions.push_back(revision);
}

void StubHistoryWriter::EndPage()
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