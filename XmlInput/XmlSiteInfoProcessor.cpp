#include "XmlSiteInfoProcessor.h"
#include "XmlNamespacesProcessor.h"
#include "../XmlUtils.h"
#include <tuple>

XmlSiteInfoProcessor::XmlSiteInfoProcessor(IDumpWriter* writer)
    : siteInfo(std::make_shared<SiteInfo>()), writer(writer)
{}

Case XmlSiteInfoProcessor::ParseCase(const std::string &caseString)
{
    if (caseString == "first-letter")
        return Case::FirstLetter;
    if (caseString == "case-sensitive")
        return Case::CaseSensitive;

    throw DumpException();
}

void XmlSiteInfoProcessor::Handler(XML::Element &elem, void *userData)
{
    XML::Handler handlers[] = {
        XML::Handler("sitename", [](XML::Element &elem, void *userData){ ((XmlSiteInfoProcessor*)userData)->siteInfo->SiteName = readElementData(elem); }),
        XML::Handler("base", [](XML::Element &elem, void *userData){ ((XmlSiteInfoProcessor*)userData)->siteInfo->Base = readElementData(elem); }),
        XML::Handler("generator", [](XML::Element &elem, void *userData){ ((XmlSiteInfoProcessor*)userData)->siteInfo->Generator = readElementData(elem); }),
        XML::Handler("case", [](XML::Element &elem, void *userData){ ((XmlSiteInfoProcessor*)userData)->siteInfo->SiteCase = ParseCase(readElementData(elem)); }),
        XML::Handler("namespaces", XmlNamespacesProcessor::Handler),
        XML::Handler::END
    };

    auto siteInfoProcessor = (XmlSiteInfoProcessor*)userData;

    elem.Process(handlers, siteInfoProcessor);

    siteInfoProcessor->writer->SetSiteInfo(siteInfoProcessor->siteInfo);
}

void XmlSiteInfoProcessor::ProcessNamespace(const std::int16_t id, const Case nsCase, const std::string name)
{
    siteInfo->Namespaces.insert(std::make_pair(id, std::make_pair(nsCase, name)));
}