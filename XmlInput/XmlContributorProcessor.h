#pragma once

#include <cstdint>
#include <string>
#include "../XML/xmlinput.h"

/**
 * Processes the @c @<contributor@> element inside @c @<revision@>.
 */
class XmlContributorProcessor
{
private:
    std::string ip;
    std::string userName;
    std::uint32_t id;

    XmlContributorProcessor()
    {}
public:
    /**
     * Handler for the @c @<contributor@> element.
     *
     * @param userData Expects this to be a pointer to Revision.
     */
    static void Handler(XML::Element &elem, void *userData);
};