#pragma once

#include "WriterWrapper.h"

/**
 * Writer wrapper that watches the progress of writing the dump.
 *
 * Progress is written to standard error output.
 */
class ProgressWriterWrapper : public WriterWrapper
{
private:
    std::uint32_t reportPeriod;

    std::uint32_t pages;
    std::uint32_t revisions;
    std::uint64_t offset;

public:
    /**
     * @param wrapped The underlying writer
     * @param reportPeriod Progress is written every @a reportPeriod revisions. If set to 0, no reporting is done.
     */
    ProgressWriterWrapper(std::unique_ptr<IDumpWriter> wrapped, std::uint32_t reportPeriod);

    virtual void StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace) OVERRIDE;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) OVERRIDE;

    virtual void SetDumpKind(DumpKind dumpKind) OVERRIDE;

    /**
     * Notifies about the current position in the input stream.
     */
    void ReportOffset(std::uint64_t offset);
};