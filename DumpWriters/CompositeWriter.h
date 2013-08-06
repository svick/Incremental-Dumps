#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "IDumpWriter.h"

class CompositeWriter : public IDumpWriter
{
private:
    std::vector<std::unique_ptr<IDumpWriter>> writers;
    std::function<std::string(std::uint32_t)> getTextFunction;
public:
    CompositeWriter(std::vector<std::unique_ptr<IDumpWriter>> &writers, std::function<std::string(std::uint32_t)> getTextFunction = nullptr)
        : writers(std::move(writers)), getTextFunction(getTextFunction)
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