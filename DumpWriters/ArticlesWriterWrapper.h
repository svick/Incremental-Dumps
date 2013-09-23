#pragma once

#include "WriterWrapper.h"

/**
 * Writer wrapper that resends only pages from specific namespaces to the underlying writer.
 *
 * Specifically, the excluded namespaces are the %User namepsace and all talk namespaces.
 */
class ArticlesWriterWrapper : public WriterWrapper
{
private:
    bool pageIncluded;
public:
    ArticlesWriterWrapper(std::unique_ptr<IDumpWriter> wrapped)
        : WriterWrapper(std::move(wrapped)), pageIncluded(false)
    {}

    virtual void StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace) OVERRIDE;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) OVERRIDE;
    virtual void EndPage() OVERRIDE;
    
    /**
     * Sets @a dumpKind combined with DumpKind::Articles as the kind of the underlying writer.
     */
    virtual void SetDumpKind(DumpKind dumpKind) OVERRIDE;
};