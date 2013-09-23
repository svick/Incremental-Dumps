#pragma once

#include <memory>
#include <vector>
#include <functional>
#include "IDumpWriter.h"
#include "../common.h"

/**
 * Composite of IDumpWriter.
 *
 * Any method that is called on this object is then called on all the writers it contains, with the same arguments.
 *
 * Also can call Revision::SetGetText() for each revision in #AddRevision().
 * (Because this is a convenient place to do so.)
 */
class CompositeWriter : public IDumpWriter
{
private:
    std::vector<std::unique_ptr<IDumpWriter>> writers;
    std::function<std::string(std::uint32_t)> getTextFunction;
public:
    /**
     * @param writers Collection of writers that are to be represented by this composite.
     * @param getTextFunction For each revision from #AddRevision(),
                              Revision::SetGetText() is called with a function that invokes this function with the revision's Revision::TextId.
     */
    CompositeWriter(std::vector<std::unique_ptr<IDumpWriter>> writers, std::function<std::string(std::uint32_t)> getTextFunction = nullptr);

    virtual void StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace) OVERRIDE;
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) OVERRIDE;
    virtual void EndPage() OVERRIDE;
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) OVERRIDE;
    virtual void SetDumpKind(DumpKind dumpKind) OVERRIDE;
    virtual void Complete() OVERRIDE;
};