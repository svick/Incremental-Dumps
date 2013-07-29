#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <utility>

enum class Case : std::uint8_t
{
    FirstLetter = 1,
    CaseSensitive = 2
};

class SiteInfo
{
public:
    SiteInfo()
    {}

    std::string Lang;
    std::string SiteName;
    std::string Base;
    std::string Generator;
    Case SiteCase;
    std::map<std::int16_t, std::pair<Case, std::string>> Namespaces;
};