#include "ArticlesWriterWrapper.h"
#include "../DumpException.h"

const std::int16_t UserNamespace = 2;

void ArticlesWriterWrapper::StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace)
{
    pageIncluded = page->Namespace % 2 == 0 && page->Namespace != UserNamespace;

    if (pageIncluded)
        wrapped->StartPage(page, titleWithNamespace);
}

void ArticlesWriterWrapper::AddRevision(const std::shared_ptr<const Revision> revision)
{
    if (pageIncluded)
        wrapped->AddRevision(revision);
}

void ArticlesWriterWrapper::EndPage()
{
    if (pageIncluded)
    {
        wrapped->EndPage();
        pageIncluded = false;
    }
}

void ArticlesWriterWrapper::SetDumpKind(DumpKind dumpKind)
{
    wrapped->SetDumpKind(dumpKind | DumpKind::Articles);
}