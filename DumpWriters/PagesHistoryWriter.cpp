#include "PagesHistoryWriter.h"
#include "../DumpObjects/DumpRevision.h"

PagesHistoryWriter::PagesHistoryWriter(shared_ptr<WritableDump> dump)
    : DumpWriter(dump)
{}

void PagesHistoryWriter::AddRevision(const shared_ptr<const Revision> revision)
{
    page->page.RevisionIds.push_back(revision->RevisionId);
    revisions.push_back(revision);
}

void PagesHistoryWriter::EndPage()
{
    page->Write();

    for (auto revision : revisions)
    {
        DumpRevision dumpRevision(dump, true);
        dumpRevision.revision = *revision;
        dumpRevision.Write();
    }

    page = nullptr;
    revisions.clear();
}