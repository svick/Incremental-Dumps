#include "ArticlesWriterWrapper.h"
#include "../DumpException.h"

const std::int16_t UserNamespace = 2;

void ArticlesWriterWrapper::StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace)
{
    pageInlcuded = page->Namespace % 2 == 0 && page->Namespace != UserNamespace;

    if (pageInlcuded)
        wrapped->StartPage(page, titleWithNamespace);
}

void ArticlesWriterWrapper::AddRevision(const std::shared_ptr<const Revision> revision)
{
    if (pageInlcuded)
        wrapped->AddRevision(revision);
}

void ArticlesWriterWrapper::EndPage()
{
    if (pageInlcuded)
    {
        wrapped->EndPage();
        pageInlcuded = false;
    }
}

void ArticlesWriterWrapper::SetDumpKind(DumpKind dumpKind)
{
    wrapped->SetDumpKind(dumpKind | DumpKind::Articles);
}