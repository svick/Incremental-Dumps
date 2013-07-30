#include "CompositeWriter.h"
#include "../DumpException.h"

void CompositeWriter::StartPage(const std::shared_ptr<const Page> page)
{
    for (auto &writer : writers)
        writer->StartPage(page);
}

const std::vector<std::uint32_t> CompositeWriter::GetRevisionIds() const
{
    throw DumpException();
}

void CompositeWriter::AddRevision(const std::shared_ptr<const Revision> revision)
{
    for (auto &writer : writers)
        writer->AddRevision(revision);
}

void CompositeWriter::DeleteRevision(std::uint32_t revisionId)
{
    for (auto &writer : writers)
        writer->DeleteRevision(revisionId);
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

void CompositeWriter::WriteIndexes()
{
    for (auto &writer : writers)
        writer->WriteIndexes();
}