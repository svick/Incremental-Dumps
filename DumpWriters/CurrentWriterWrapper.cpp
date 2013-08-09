#include "CurrentWriterWrapper.h"

void CurrentWriterWrapper::AddRevision(const std::shared_ptr<const Revision> revision)
{
    this->revision = revision;
}

void CurrentWriterWrapper::EndPage()
{
    wrapped->AddRevision(this->revision);
    wrapped->EndPage();

    revision = nullptr;
}

void CurrentWriterWrapper::SetDumpKind(DumpKind dumpKind)
{
    wrapped->SetDumpKind(dumpKind | DumpKind::Current);
}