#pragma once

#include <memory>
#include <vector>
#include "IDumpWriter.h"

class CompositeWriter : public IDumpWriter
{
private:
    std::vector<std::unique_ptr<IDumpWriter>> writers;
public:
    CompositeWriter(std::vector<std::unique_ptr<IDumpWriter>> &writers)
        : writers(std::move(writers))
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