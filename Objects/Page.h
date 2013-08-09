#pragma once

#include <cstdint>
#include <string>
#include <vector>

class Page
{
public:
    std::uint32_t PageId;
    std::int16_t Namespace;
    std::string Title;
    // if empty, the page is not a redirect
    std::string RedirectTarget;

    std::vector<std::uint32_t> RevisionIds;

    Page();
};

bool operator ==(const Page &first, const Page &second);