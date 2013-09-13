#pragma once

#include "WriterWrapper.h"

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
    virtual void SetDumpKind(DumpKind dumpKind) OVERRIDE;
};