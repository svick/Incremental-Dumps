#include "XmlContributorProcessor.h"
#include "../XmlUtils.h"
#include "../Objects/Revision.h"
#include "../Objects/User.h"
#include "../Objects/NamedUser.h"

void XmlContributorProcessor::Handler(XML::Element &elem, void *userData)
{
    auto revision = (Revision*)userData;

    if (elem.GetAttribute("deleted") != nullptr)
    {
        revision->Flags |= RevisionFlags::ContributorDeleted;
        return;
    }

    XML::Handler handlers[] = {
        XML::Handler("ip", [](XML::Element &elem, void *userData) { ((XmlContributorProcessor*)userData)->ip = readElementData(elem); }),

        XML::Handler("username", [](XML::Element &elem, void *userData) { ((XmlContributorProcessor*)userData)->userName = readElementData(elem); }),
        XML::Handler("id", [](XML::Element &elem, void *userData) { ((XmlContributorProcessor*)userData)->id = stoi(readElementData(elem)); }),

        XML::Handler::END
    };
    
    XmlContributorProcessor processor;

    elem.Process(handlers, &processor);

    std::shared_ptr<User> user;

    if (processor.ip != string())
        user = User::CreateFromIp(processor.ip);
    else
        user = std::make_shared<NamedUser>(processor.id, processor.userName);

    revision->Contributor = std::shared_ptr<User>(user);
    revision->Flags |= user->UserKind();
}