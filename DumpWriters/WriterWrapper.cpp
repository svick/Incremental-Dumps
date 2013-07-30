#include "WriterWrapper.h"

void WriterWrapper::StartPage(const std::shared_ptr<const Page> page)
{
    wrapped->StartPage(page);
}

const std::vector<std::uint32_t> WriterWrapper::GetRevisionIds() const
{
    return wrapped->GetRevisionIds();
}

void WriterWrapper::AddRevision(const std::shared_ptr<const Revision> revision)
{
    wrapped->AddRevision(revision);
}

void WriterWrapper::DeleteRevision(std::uint32_t revisionId)
{
    wrapped->DeleteRevision(revisionId);
}

void WriterWrapper::EndPage()
{
    wrapped->EndPage();
}

void WriterWrapper::SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo)
{
    wrapped->SetSiteInfo(siteInfo);
}

void WriterWrapper::WriteIndexes()
{
    wrapped->WriteIndexes();
}