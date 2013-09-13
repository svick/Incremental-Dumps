#pragma once

#include "IDumpWriter.h"
#include "../common.h"

class WriterWrapper : public IDumpWriter
{
protected:
    std::unique_ptr<IDumpWriter> wrapped;
public:
    WriterWrapper(std::unique_ptr<IDumpWriter> wrapped)
        : wrapped(std::move(wrapped))
    {}

    virtual void StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace) OVERRIDE;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) OVERRIDE;
    virtual void EndPage() OVERRIDE;
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) OVERRIDE;
    virtual void SetDumpKind(DumpKind dumpKind) OVERRIDE = 0;
    virtual void Complete() OVERRIDE;
};