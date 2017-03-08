#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <utility>

/**
 * The case of page titles in some namespace.
 */
enum class Case : std::uint8_t
{
    FirstLetter = 1,
    CaseSensitive = 2
};

/**
 * Contains information about the whole wiki site.
 */
class SiteInfo
{
public:
    SiteInfo()
    {}
    
    /**
     * XML lang tag for the site (e.g. "en")
     */
    std::string Lang;

    /**
     * Name of the site (e.g. "Wikipedia")
     */
    std::string SiteName;

    /*
     * Name of the database (e.g. "enwiki")
     */
    std::string DbName;

    /**
     * URL to the main page of the site.
     */
    std::string Base;

    /**
     * Version of the wiki the data in this dump comes from (e.g. "MediaWiki 1.22wmf13").
     */
    std::string Generator;

    Case SiteCase;

    /**
     * List of namespaces in the wiki.
     *
     * The key is the id of the namespace, the value is a pair of namespace case and namespace name.
     */
    std::map<std::int16_t, std::pair<Case, std::string>> Namespaces;
};