#include <iostream>
#include "XML/xmlinput.h"
#include "XML/xmlfile.h"
#include "DumpWriters/StubCurrentWriter.h"
#include "XmlPageProcessor.h"
#include "Dump.h"

using std::cin;

class StandardInputStream : public XML::InputStream
{
public:
    virtual int read(XML_Char *buf, size_t bufLen)
    {
        cin.read(buf, bufLen);
        return cin.gcount();
    }
};

void mediawikiHandler(XML::Element &elem, void *userData)
{
    XML::Handler handlers[] = {
        XML::Handler("page", XmlPageProcessor::Handler),
        XML::Handler::END
    };

    elem.Process(handlers, userData);
}

int main(int argc, const char* argv[])
{
    //StandardInputStream stream;
    XML::FileInputStream stream = XML::FileInputStream("C:\\Users\\Svick\\Downloads\\tenwiki-20130622-pages-meta-history.xml");

    XML::Input input(stream);

    XML::Handler handlers[] = {
        XML::Handler(mediawikiHandler),
        XML::Handler::END
    };

    shared_ptr<WritableDump> dump = WritableDump::Create("tmp/test.id");

    StubCurrentWriter writer(dump);

    input.Process(handlers, &writer);

    /*shared_ptr<WritableDump> dump = WritableDump::Create("tmp/test.id");

    auto offset = dump->spaceManager->GetSpace(102);
    dump->spaceManager->Delete(offset, 102);

    dump->pageIdIndex->Add(1, 2);*/
}