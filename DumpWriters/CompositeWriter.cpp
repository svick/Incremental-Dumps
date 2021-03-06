#include "CompositeWriter.h"
#include "../DumpException.h"

CompositeWriter::CompositeWriter(std::vector<std::unique_ptr<IDumpWriter>> writers, std::function<std::string(std::uint32_t)> getTextFunction)
    : writers(std::move(writers)), getTextFunction(getTextFunction)
{}

void CompositeWriter::StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace)
{
    for (auto &writer : writers)
        writer->StartPage(page, titleWithNamespace);
}

void CompositeWriter::AddRevision(const std::shared_ptr<const Revision> revision)
{
    if (getTextFunction != nullptr)
        const_cast<Revision&>(*revision).SetGetText([=](){ return getTextFunction(revision->TextId); });

    for (auto &writer : writers)
        writer->AddRevision(revision);
}

void CompositeWriter::EndPage()
{
    for (auto &writer : writers)
        writer->EndPage();
}

void CompositeWriter::SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo)
{
    for (auto &writer : writers)
        writer->SetSiteInfo(siteInfo);
}

void CompositeWriter::SetDumpKind(DumpKind dumpKind)
{
    for (auto &writer : writers)
        writer->SetDumpKind(dumpKind);
}

void CompositeWriter::Complete()
{
    for (auto &writer : writers)
        writer->Complete();
}