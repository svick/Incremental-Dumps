#include "XmlPageProcessor.h"
#include "XmlRevisionProcessor.h"
#include "XmlUtils.h"
#include "DumpWriters/DumpWriter.h"

using std::make_shared;
using std::stoi;

void XmlPageProcessor::writePage()
{
    if (!pageWritten)
    {
        dumpWriter->StartPage(page);
        pageWritten = true;
    }
}

void XmlPageProcessor::completePage()
{
    dumpWriter->EndPage();
}

XmlPageProcessor::XmlPageProcessor(const shared_ptr<Page> page, DumpWriter* dumpWriter)
    : page(page), dumpWriter(dumpWriter), pageWritten(false)
{}

void XmlPageProcessor::Handler(XML::Element &elem, void *userData)
{
    XML::Handler handlers[] = {
        XML::Handler("title", [](XML::Element &elem, void *userData){ ((XmlPageProcessor*)userData)->page->Title = readElementData(elem); }),
        XML::Handler("ns", [](XML::Element &elem, void *userData){ ((XmlPageProcessor*)userData)->page->Namespace = stoi(readElementData(elem)); }),
        XML::Handler("id", [](XML::Element &elem, void *userData){ ((XmlPageProcessor*)userData)->page->PageId = stoi(readElementData(elem)); }),
        XML::Handler("redirect", [](XML::Element &elem, void *userData){ ((XmlPageProcessor*)userData)->page->RedirectTarget = string(elem.GetAttribute("title")); }),
        XML::Handler("revision", XmlRevisionProcessor::Handler),
        XML::Handler::END
    };

    XmlPageProcessor pageProcessor(make_shared<Page>(), (DumpWriter*)userData);

    elem.Process(handlers, &pageProcessor);

    pageProcessor.writePage();
    pageProcessor.completePage();
}

void XmlPageProcessor::ProcessRevision(const shared_ptr<const Revision> revision)
{
    writePage();
    dumpWriter->AddRevision(revision);
}