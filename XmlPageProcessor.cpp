#include "XmlPageProcessor.h"
#include "XmlRevisionProcessor.h"
#include "XmlUtils.h"
#include "DumpWriter.h"

using std::make_shared;

void XmlPageProcessor::titleHandler(XML::Element &elem, void *userData)
{
    XmlPageProcessor* processor = (XmlPageProcessor*)userData;
    processor->page->Title = readElementData(elem);
}

void XmlPageProcessor::writePage()
{
    if (!pageWritten)
    {
        dumpWriter->WritePage(page);
        pageWritten = true;
    }
}

XmlPageProcessor::XmlPageProcessor(const shared_ptr<Page> page, DumpWriter* dumpWriter)
    : page(page), dumpWriter(dumpWriter), pageWritten(false)
{
}

void XmlPageProcessor::Handler(XML::Element &elem, void *userData)
{
    static int i = 0;

    if (i++ > 5)
        return;

    XML::Handler handlers[] = {
        XML::Handler("title", titleHandler),
        XML::Handler("revision", XmlRevisionProcessor::Handler),
        XML::Handler::END
    };

    XmlRevisionProcessor::revisionIndex = 0;

    XmlPageProcessor pageProcessor(make_shared<Page>(), (DumpWriter*)userData);

    elem.Process(handlers, &pageProcessor);

    pageProcessor.writePage();
}

void XmlPageProcessor::ProcessRevision(const shared_ptr<const Revision> revision)
{
    writePage();
    dumpWriter->WriteRevision(revision);
}