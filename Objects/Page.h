#pragma once

#include <cstdint>
#include <string>
#include <vector>

using std::uint16_t;
using std::uint32_t;
using std::string;
using std::vector;

class Page
{
public:
    uint32_t PageId;
    uint16_t Namespace;
    string Title;
    // if empty, the page is not a redirect
    string RedirectTarget;

    vector<uint32_t> RevisionIds;

    Page();
};