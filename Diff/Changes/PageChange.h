#pragma once

#include "Change.h"
#include "../../Objects/Page.h"

/** @file */

/**
* Flags that indicate which parts of page changed.
*/
enum class PageChangeFlags : std::uint8_t
{
    NoChanges             = 0x00,

    NamespaceChanged      = 0x01,
    TitleChanged          = 0x02,
    RedirectTargetChanged = 0x04
};

PageChangeFlags operator |(PageChangeFlags first, PageChangeFlags second);
PageChangeFlags operator |=(PageChangeFlags &first, PageChangeFlags second);
bool HasFlag(PageChangeFlags value, PageChangeFlags flag);

/**
 * A change that represents change in a page in the dump.
 *
 * This can be used for changes in metadata of the page itself,
 * or changes in the list of revisions (represented by following revision changes),
 * or both.
 */
class PageChange : public Change
{
private:
    PageChange();
public:
    /**
     * Which parts of metadata of the page changed.
     */
    PageChangeFlags flags;
    /**
     * Incomplete Page object that contains only the changed fields.
     */
    Page pageChanges;

    /**
     * Creates a page change based on two versions of the same page.
     */
    PageChange(const Page &oldPage, const Page &newPage);
    /**
     * Creates a page change that doesn't contain any changes in the metadata of the page.
     */
    PageChange(std::uint32_t pageId);

    /**
     * Whether this change contains any changes in the metadata of the page.
     */
    bool HasChanges();

    static PageChange Read(std::istream &stream);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};