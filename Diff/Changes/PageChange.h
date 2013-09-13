#pragma once

#include "Change.h"
#include "../../Objects/Page.h"

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

class PageChange : public Change
{
private:
    PageChange();
public:
    PageChangeFlags flags;
    Page pageChanges;

    PageChange(const Page &oldPage, const Page &newPage);
    // if it's known that the page itself didn't change
    PageChange(std::uint32_t pageId);

    bool HasChanges();

    static PageChange Read(std::istream &stream);
    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;
};