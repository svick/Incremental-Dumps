#include "Page.h"

Page::Page()
    : PageId(), Namespace()
{}

bool operator ==(const Page &first, const Page &second)
{
    return first.PageId == second.PageId
        && first.Namespace == second.Namespace
        && first.Title == second.Title
        && first.RedirectTarget == second.RedirectTarget
        && first.Restrictions == second.Restrictions
        && first.RevisionIds == second.RevisionIds;
}