#include "XmlRevisionProcessor.h"
#include "XmlPageProcessor.h"
#include "XmlUtils.h"

int XmlRevisionProcessor::revisionIndex = 0;

void XmlRevisionProcessor::textHandler(XML::Element &elem, void *userData)
{
    ((Revision*)userData)->Text = readElementData(elem);
}

void XmlRevisionProcessor::Handler(XML::Element &elem, void *userData)
{
    if (revisionIndex++ >= 5)
        return;

    XML::Handler handlers[] = {
        XML::Handler("text", textHandler),
        XML::Handler::END
    };

    Revision* revision = new Revision;

    elem.Process(handlers, revision);

    XmlPageProcessor* pageProcessor = (XmlPageProcessor*)userData;

    pageProcessor->ProcessRevision(shared_ptr<Revision>(revision));
}