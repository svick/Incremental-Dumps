#pragma once

#include <memory>
#include "../XML/xmlinput.h"
#include "../Objects/SiteInfo.h"
#include "../DumpWriters/DumpWriter.h"

class XmlSiteInfoProcessor
{
private:
    DumpWriter *writer;
public:
    shared_ptr<SiteInfo> siteInfo;

    XmlSiteInfoProcessor(DumpWriter *writer);

    static Case ParseCase(const std::string &caseString);
    static void Handler(XML::Element &elem, void *userData);

    void ProcessNamespace(const std::int16_t id, const Case nsCase, const std::string name);
};