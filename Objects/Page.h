#pragma once

#include <string>

using std::string;

class Page
{
public:
    uint32_t PageId;
    uint16_t Namespace;
    string Title;
    // if empty, the page is not a redirect
    string RedirectTarget;
};