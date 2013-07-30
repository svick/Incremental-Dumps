#pragma once

#include "IDumpWriter.h"

class WriterWrapper : public IDumpWriter
{
protected:
    std::unique_ptr<IDumpWriter> wrapped;
public:
    WriterWrapper(std::unique_ptr<IDumpWriter> wrapped)
        : wrapped(std::move(wrapped))
    {}

    virtual void StartPage(const std::shared_ptr<const Page> page) override;
    virtual const std::vector<std::uint32_t> GetRevisionIds() const override;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) override;
    virtual void DeleteRevision(std::uint32_t revisionId) override;
    virtual void EndPage() override;
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) override;
    virtual void SetDumpKind(DumpKind dumpKind) override = 0;
    virtual void WriteIndexes() override;
};