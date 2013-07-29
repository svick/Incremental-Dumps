#include "XmlNamespacesProcessor.h"
#include "XmlSiteInfoProcessor.h"
#include "../XmlUtils.h"
#include "../DumpWriters/DumpWriter.h"

void XmlNamespacesProcessor::Handler(XML::Element &elem, void *userData)
{
    XML::Handler handlers[] = {
        XML::Handler("namespace",
            [](XML::Element &elem, void *userData)
            {
                auto siteInfoProcessor = (XmlSiteInfoProcessor*)userData;

                int id;
                Case nsCase;
                std::string name;

                elem.GetAttribute("key", id);
                nsCase = XmlSiteInfoProcessor::ParseCase(elem.GetAttribute("case"));
                name = readElementData(elem);

                siteInfoProcessor->ProcessNamespace(id, nsCase, name);
            }),
        XML::Handler::END
    };

    elem.Process(handlers, userData);
}