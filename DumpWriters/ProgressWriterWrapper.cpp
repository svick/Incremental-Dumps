#include "ProgressWriterWrapper.h"
#include "../format.h"

ProgressWriterWrapper::ProgressWriterWrapper(std::unique_ptr<IDumpWriter> wrapped, std::uint32_t reportPeriod)
    : WriterWrapper(std::move(wrapped)), reportPeriod(reportPeriod), pages(0), revisions(0), offset(0)
{ }

void ProgressWriterWrapper::StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace)
{
    pages++;

    WriterWrapper::StartPage(page, titleWithNamespace);
}

void ProgressWriterWrapper::AddRevision(const std::shared_ptr<const Revision> revision)
{
    revisions++;

    if (reportPeriod != 0 && revisions % reportPeriod == 0)
        std::cerr << str(fmt::Format("{0} revs, {1} pages, {2} MB\n") << revisions << pages << (offset / 1024 / 1024));

    WriterWrapper::AddRevision(revision);
}

void ProgressWriterWrapper::SetDumpKind(DumpKind dumpKind)
{
    wrapped->SetDumpKind(dumpKind);
}

void ProgressWriterWrapper::ReportOffset(std::uint64_t offset)
{
    this->offset = offset;
}