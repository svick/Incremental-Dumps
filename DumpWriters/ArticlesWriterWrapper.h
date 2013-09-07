#pragma once

#include "WriterWrapper.h"

class ArticlesWriterWrapper : public WriterWrapper
{
private:
    bool pageInlcuded;
public:
    ArticlesWriterWrapper(std::unique_ptr<IDumpWriter> wrapped)
        : WriterWrapper(std::move(wrapped)), pageInlcuded(false)
    {}

    virtual void StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace) override;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) override;
    virtual void EndPage() override;
    virtual void SetDumpKind(DumpKind dumpKind) override;
};