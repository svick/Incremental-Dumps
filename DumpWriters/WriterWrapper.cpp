#include "WriterWrapper.h"

void WriterWrapper::StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace)
{
    wrapped->StartPage(page, titleWithNamespace);
}

void WriterWrapper::AddRevision(const std::shared_ptr<const Revision> revision)
{
    wrapped->AddRevision(revision);
}

void WriterWrapper::EndPage()
{
    wrapped->EndPage();
}

void WriterWrapper::SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo)
{
    wrapped->SetSiteInfo(siteInfo);
}

void WriterWrapper::EndDump()
{
    wrapped->EndDump();
}