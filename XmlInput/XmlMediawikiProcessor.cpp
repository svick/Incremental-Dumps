#include "XmlMediawikiProcessor.h"
#include "XmlPageProcessor.h"
#include "XmlSiteInfoProcessor.h"
#include "../XML/xmlfile.h"

void mediawikiHandler(XML::Element &elem, void *userData)
{
    auto writer = (IDumpWriter*)userData;

    std::string lang = elem.GetAttribute("xml:lang");

    XmlSiteInfoProcessor siteInfoProcessor(writer);
    siteInfoProcessor.siteInfo->Lang = lang;

    XML::Handler handlers[] = {
        XML::Handler("page", XmlPageProcessor::Handler),
        XML::Handler("siteinfo", XmlSiteInfoProcessor::Handler, &siteInfoProcessor),
        XML::Handler::END
    };

    elem.Process(handlers, writer);
}

void XmlMediawikiProcessor::Process(IDumpWriter *writer, XML::InputStream &inputStream)
{
    XML::Input input(inputStream);

    XML::Handler handlers[] = {
        XML::Handler(mediawikiHandler),
        XML::Handler::END
    };

    input.Process(handlers, writer);
}

void XmlMediawikiProcessor::Process(IDumpWriter *writer, std::string inputFileName)
{
    XML::FileInputStream stream = XML::FileInputStream(inputFileName.c_str());

    Process(writer, stream);
}