#include "XmlPageProcessor.h"
#include "XmlRevisionProcessor.h"
#include "XmlUtils.h"
#include "DumpWriters/DumpWriter.h"

using std::make_shared;
using std::stoi;

void XmlPageProcessor::titleHandler(XML::Element &elem, void *userData)
{
    XmlPageProcessor* processor = (XmlPageProcessor*)userData;
    processor->page->Title = readElementData(elem);
}

void XmlPageProcessor::nsHandler(XML::Element &elem, void *userData)
{
    XmlPageProcessor* processor = (XmlPageProcessor*)userData;
    processor->page->Namespace = stoi(readElementData(elem));
}

void XmlPageProcessor::idHandler(XML::Element &elem, void *userData)
{
    XmlPageProcessor* processor = (XmlPageProcessor*)userData;
    processor->page->PageId = stoi(readElementData(elem));
}

void XmlPageProcessor::redirectHandler(XML::Element &elem, void *userData)
{
    XmlPageProcessor* processor = (XmlPageProcessor*)userData;
    
    //processor->page->RedirectTarget = readElementData(elem);
    processor->page->RedirectTarget = string(elem.GetAttribute("title"));
}

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
{
}

void XmlPageProcessor::Handler(XML::Element &elem, void *userData)
{
    static int i = 0;

    if (i++ >= 255)
        return;

    XML::Handler handlers[] = {
        XML::Handler("title", titleHandler),
        XML::Handler("ns", nsHandler),
        XML::Handler("id", idHandler),
        XML::Handler("redirect", redirectHandler),
        XML::Handler("revision", XmlRevisionProcessor::Handler),
        XML::Handler::END
    };

    XmlRevisionProcessor::revisionIndex = 0;

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