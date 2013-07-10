#include "StubCurrentWriter.h"

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
}

void StubCurrentWriter::EndPage()
{
    page->Write();
    page = nullptr;
}