#pragma once

#include "IDumpWriter.h"
#include "../DumpObjects/DumpPage.h"
#include "../Dump.h"
#include "../DumpObjects/DumpPage.h"


class DumpWriter : public IDumpWriter
{
private:
    std::shared_ptr<WritableDump> dump;
    unique_ptr<DumpPage> page;
    vector<shared_ptr<const Revision>> revisions;
    bool withText;

public:
    DumpWriter(std::shared_ptr<WritableDump> dump, bool withText)
        : dump(dump), withText(withText)
    {}

    virtual void StartPage(const std::shared_ptr<const Page> page) override;
    virtual const std::vector<std::uint32_t> GetRevisionIds() const override;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) override;
    virtual void DeleteRevision(std::uint32_t revisionId) override;
    virtual void EndPage() override;
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) override;
    virtual void SetDumpKind(DumpKind dumpKind) override;
    virtual void WriteIndexes() override;
};