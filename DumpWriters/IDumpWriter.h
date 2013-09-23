#pragma once

#include <memory>
#include "../Objects/Page.h"
#include "../Objects/Revision.h"
#include "../Objects/SiteInfo.h"
#include "../DumpKind.h"

/**
 * An "interface" for dump writers.
 * Object that derives from this class is used to update a dump
 * by feeding it all pages and revisions that should be present in the updated version of the dump.
 */
class IDumpWriter
{
public:
    /**
     * Calling this method means that @a page should be included in the dump.
     *
     * It also sets this page as current,
     * which means that all revisions from calls to #AddRevision() from now until a call to #EndPage() belong to this page.
     *
     * @param page The page as it should appear in the dump. Its Page::RevisionIds has to be empty.
     * @param titleWithNamespace Whether Page::Title of the page contains the namespace name (followed by colon).
     */
    virtual void StartPage(const std::shared_ptr<const Page> page, bool titleWithNamespace) = 0;

    /**
     * Calling this method means that @a revision should be included in the dump
     * and that it belongs to the current page (specified by calling #StartPage()).
     */
    virtual void AddRevision(const std::shared_ptr<const Revision> revision) = 0;

    /**
     * Resets the current page.
     *
     * Calls to #AddRevision() after this method is called (and before #StartPage() is called again) are invalid.
     */
    virtual void EndPage() = 0;

    /**
     * Sets the site info of the dump to the given value.
     *
     * If the dump is not new, its old site info will be overwritten.
     */
    virtual void SetSiteInfo(const std::shared_ptr<const SiteInfo> siteInfo) = 0;

    /**
     * Sets the kind of the dump to the given value.
     *
     * If the dump is not new and the kind doesn't match, an exception is thrown.
     */
    virtual void SetDumpKind(DumpKind dumpKind) = 0;

    /**
     * Completes writing to the dump.
     *
     * This method has to be called after writing is complete.
     */
    virtual void Complete() = 0;

    virtual ~IDumpWriter() {}
};
