#pragma once

#include <memory>
#include "../Objects/Page.h"
#include "../Objects/Revision.h"
#include "../Objects/SiteInfo.h"
#include "../DumpKind.h"

class IDumpWriter
{
public:
    virtual void StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace) = 0;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) = 0;
    virtual void EndPage() = 0;
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) = 0;
    virtual void SetDumpKind(DumpKind dumpKind) = 0;
    virtual void Complete() = 0;

    virtual ~IDumpWriter() {}
};
