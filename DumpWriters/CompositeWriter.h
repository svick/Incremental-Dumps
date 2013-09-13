#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "IDumpWriter.h"
#include "../common.h"

class CompositeWriter : public IDumpWriter
{
private:
    std::vector<std::unique_ptr<IDumpWriter>> writers;
    std::function<std::string(std::uint32_t)> getTextFunction;
public:
    CompositeWriter(std::vector<std::unique_ptr<IDumpWriter>> &writers, std::function<std::string(std::uint32_t)> getTextFunction = nullptr)
        : writers(std::move(writers)), getTextFunction(getTextFunction)
    {}

    virtual void StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace) OVERRIDE;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) OVERRIDE;
    virtual void EndPage() OVERRIDE;
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) OVERRIDE;
    virtual void SetDumpKind(DumpKind dumpKind) OVERRIDE;
    virtual void Complete() OVERRIDE;
};