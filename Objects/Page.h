#pragma once

#include <cstdint>
#include <string>
#include <set>

/**
 * A page in the wiki.
 */
class Page
{
public:
    /**
     * Unique ID of the page.
     */
    std::uint32_t PageId;

    /**
     * ID of the namespace the page is in.
     */
    std::int16_t Namespace;

    /**
     * Title of the page.
     *
     * May or may not include the namespace name.
     */
    std::string Title;

    /**
     * Name of the page this page redirects to, including namespace.
     *
     * If it's empty, this page is not a redirect.
     */
    std::string RedirectTarget;

    /**
     * List of IDs of revisions that belong to this page.
     *
     * The type is std::set to keep the revisions sorted by ID.
     */
    std::set<std::uint32_t> RevisionIds;

    Page();
};

bool operator ==(const Page &first, const Page &second);