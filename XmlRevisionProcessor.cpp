#include "XmlRevisionProcessor.h"
#include "XmlPageProcessor.h"
#include "XmlContributorProcessor.h"
#include "XmlUtils.h"
#include "Objects/Timestamp.h"

void XmlRevisionProcessor::Handler(XML::Element &elem, void *userData)
{
    XML::Handler handlers[] = {
        XML::Handler("id", [](XML::Element &elem, void *userData) { ((Revision*)userData)->RevisionId = stoi(readElementData(elem)); }),
        XML::Handler("parentid", [](XML::Element &elem, void *userData) { ((Revision*)userData)->ParentId = stoi(readElementData(elem)); }),

        XML::Handler("timestamp", [](XML::Element &elem, void *userData) { ((Revision*)userData)->Timestamp = Timestamp(readElementData(elem)).ToInteger(); }),

        XML::Handler("contributor", XmlContributorProcessor::Handler),

        XML::Handler("minor", [](XML::Element &elem, void *userData) { ((Revision*)userData)->Flags |= RevisionFlags::Minor; }),

        XML::Handler("comment", [](XML::Element &elem, void *userData) { ((Revision*)userData)->Comment = readElementData(elem); }),
        XML::Handler("text", [](XML::Element &elem, void *userData) { ((Revision*)userData)->Text = readElementData(elem); }),

        XML::Handler::END
    };

    Revision* revision = new Revision;

    elem.Process(handlers, revision);

    XmlPageProcessor* pageProcessor = (XmlPageProcessor*)userData;

    pageProcessor->ProcessRevision(shared_ptr<Revision>(revision));
}