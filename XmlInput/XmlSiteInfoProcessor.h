#pragma once

#include <memory>
#include "../XML/xmlinput.h"
#include "../Objects/SiteInfo.h"
#include "../DumpWriters/DumpWriter.h"

/**
 * Processes the @c @<siteinfo@> element.
 */
class XmlSiteInfoProcessor
{
private:
    IDumpWriter *writer;

    std::shared_ptr<SiteInfo> siteInfo;
public:
    /**
     * @param lang Language tag for the wiki (the value of @c xml:lang of the root element).
     */
    XmlSiteInfoProcessor(IDumpWriter *writer, std::string lang);

    /**
     * Parses the string representation of case from XML to Case.
     */
    static Case ParseCase(std::string caseString);

    /**
     * Handler for the @c @<siteinfo@> element.
     *
     * @param userData Expects this to be a pointer to XmlSiteInfoProcessor.
     */
    static void Handler(XML::Element &elem, void *userData);

    /**
     * Processes information about namespace.
     *
     * Called from XmlNamespacesProcessor.
     */
    void ProcessNamespace(std::int16_t id, Case nsCase, std::string name);
};