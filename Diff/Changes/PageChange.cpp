#include "PageChange.h"

PageChangeFlags operator |(PageChangeFlags first, PageChangeFlags second)
{
    return (PageChangeFlags)((std::uint8_t)first | (std::uint8_t)second);
}

PageChangeFlags operator |=(PageChangeFlags &first, PageChangeFlags second)
{
    first = first | second;
    return first;
}

bool HasFlag(PageChangeFlags value, PageChangeFlags flag)
{
    return ((std::uint8_t)value & (std::uint8_t)flag) != 0;
}

PageChange::PageChange()
{}

PageChange::PageChange(const Page &oldPage, const Page &newPage)
{
    pageChanges = newPage;
    // revision ids are not needed here, so we can save some memory
    pageChanges.RevisionIds = std::set<std::uint32_t>();

    flags = PageChangeFlags::NoChanges;

    if (oldPage.Namespace != newPage.Namespace)
        flags |= PageChangeFlags::NamespaceChanged;
    if (oldPage.Title != newPage.Title)
        flags |= PageChangeFlags::TitleChanged;
    if (oldPage.RedirectTarget != newPage.RedirectTarget)
        flags |= PageChangeFlags::RedirectTargetChanged;
    if (oldPage.Restrictions != newPage.Restrictions)
        flags |= PageChangeFlags::RestrictionsChanged;
}

PageChange::PageChange(std::uint32_t pageId)
{
    pageChanges.PageId = pageId;

    flags = PageChangeFlags::NoChanges;
}

bool PageChange::HasChanges()
{
    return flags != PageChangeFlags::NoChanges;
}

PageChange PageChange::Read(std::istream &stream)
{
    PageChange result;

    ReadValue(stream, result.pageChanges.PageId);
    ReadValue(stream, result.flags);

    if (HasFlag(result.flags, PageChangeFlags::NamespaceChanged))
        ReadValue(stream, result.pageChanges.Namespace);

    if (HasFlag(result.flags, PageChangeFlags::TitleChanged))
        ReadValue(stream, result.pageChanges.Title);

    if (HasFlag(result.flags, PageChangeFlags::RedirectTargetChanged))
        ReadValue(stream, result.pageChanges.RedirectTarget);

    if (HasFlag(result.flags, PageChangeFlags::RestrictionsChanged))
        ReadValue(stream, result.pageChanges.Restrictions);

    return result;
}

void PageChange::WriteInternal()
{
    WriteValue(ChangeKind::ChangePage);
    WriteValue(pageChanges.PageId);
    WriteValue(flags);

    if (HasFlag(flags, PageChangeFlags::NamespaceChanged))
        WriteValue(pageChanges.Namespace);

    if (HasFlag(flags, PageChangeFlags::TitleChanged))
        WriteValue(pageChanges.Title);

    if (HasFlag(flags, PageChangeFlags::RedirectTargetChanged))
        WriteValue(pageChanges.RedirectTarget);

    if (HasFlag(flags, PageChangeFlags::RestrictionsChanged))
        WriteValue(pageChanges.Restrictions);
}

std::uint32_t PageChange::NewLength()
{
    std::uint32_t result = 0;

    result += ValueSize(ChangeKind::ChangePage);
    result += ValueSize(pageChanges.PageId);
    result += ValueSize(flags);

    if (HasFlag(flags, PageChangeFlags::NamespaceChanged))
        result += ValueSize(pageChanges.Namespace);
    if (HasFlag(flags, PageChangeFlags::TitleChanged))
        result += ValueSize(pageChanges.Title);
    if (HasFlag(flags, PageChangeFlags::RedirectTargetChanged))
        result += ValueSize(pageChanges.RedirectTarget);
    if (HasFlag(flags, PageChangeFlags::RestrictionsChanged))
        result += ValueSize(pageChanges.Restrictions);

    return result;
}