#pragma once

#include <memory>
#include "../Objects/Page.h"
#include "../Objects/Revision.h"
#include "../Objects/SiteInfo.h"
#include "../Dump.h"
#include "../DumpObjects/DumpPage.h"

class DumpWriterBase
{
public:
    virtual void StartPage(const std::shared_ptr<const Page> page) = 0;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) = 0;
    virtual void EndPage() = 0;
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) = 0;
};

class DumpWriter : public DumpWriterBase
{
protected:
    std::shared_ptr<WritableDump> dump;
    unique_ptr<DumpPage> page;

public:
    DumpWriter(std::shared_ptr<WritableDump> dump)
        : dump(dump)
    {}

    virtual void StartPage(const std::shared_ptr<const Page> page) override;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) override = 0;
    virtual void EndPage() override = 0;
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) override;
};