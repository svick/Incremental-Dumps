#pragma once

#include "WriterWrapper.h"

class ProgressWriterWrapper : public WriterWrapper
{
private:
    std::uint32_t reportPeriod;

    std::uint32_t pages;
    std::uint32_t revisions;
    std::uint64_t offset;

public:
    ProgressWriterWrapper(std::unique_ptr<IDumpWriter> wrapped, std::uint32_t reportPeriod);

    virtual void StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace) OVERRIDE;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) OVERRIDE;

    virtual void SetDumpKind(DumpKind dumpKind) OVERRIDE;

    void ReportOffset(std::uint64_t offset);
};