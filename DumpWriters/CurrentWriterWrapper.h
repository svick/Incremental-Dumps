#pragma once

#include "WriterWrapper.h"

/**
 * Writer wrapper that resends only the most recent revision of each page to the underlying writer.
 */
class CurrentWriterWrapper : public WriterWrapper
{
private:
    std::shared_ptr<const Revision> revision;
public:
    CurrentWriterWrapper(std::unique_ptr<IDumpWriter> wrapped)
        : WriterWrapper(std::move(wrapped))
    {}

    virtual void AddRevision(const std::shared_ptr<const Revision> revision) OVERRIDE;
    virtual void EndPage() OVERRIDE;

    /**
     * Sets @a dumpKind combined with DumpKind::Current as the kind of the underlying writer.
     */
    virtual void SetDumpKind(DumpKind dumpKind) OVERRIDE;
};