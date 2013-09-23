#pragma once

#include "../XML/xmlinput.h"

/**
 * Processes the @c @<namespaces@> element inside @c @<siteinfo@>.
 */
class XmlNamespacesProcessor
{
private:
    XmlNamespacesProcessor()
    {}
public:
    /**
     * Handler for the @c @<namespaces@> element.
     *
     * @param userData Expects this to be a pointer to XmlSiteInfoProcessor.
     */
    static void Handler(XML::Element &elem, void *userData);
};