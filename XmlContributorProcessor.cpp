#include "XmlContributorProcessor.h"
#include "XmlUtils.h"
#include "Objects/Revision.h"
#include "Objects/IpV4User.h"

void XmlContributorProcessor::Handler(XML::Element &elem, void *userData)
{
    XML::Handler handlers[] = {
        XML::Handler("ip", [](XML::Element &elem, void *userData) { ((XmlContributorProcessor*)userData)->ip = readElementData(elem); }),

        XML::Handler("username", [](XML::Element &elem, void *userData) { ((XmlContributorProcessor*)userData)->userName = readElementData(elem); }),
        XML::Handler("id", [](XML::Element &elem, void *userData) { ((XmlContributorProcessor*)userData)->id = stoi(readElementData(elem)); }),

        XML::Handler::END
    };

    XmlContributorProcessor processor;

    elem.Process(handlers, &processor);

    auto revision = (Revision*)userData;

    User* user;

    if (processor.ip != string())
        user = new IpV4User(processor.ip);
    else
        user = new User(processor.id, processor.userName);

    revision->Contributor = shared_ptr<User>(user);
}