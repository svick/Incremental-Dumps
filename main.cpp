#include <string>
#include <iostream>
#include "XML/xmlinput.h"
#include "XML/xmlfile.h"
#include "TestDumpWriter.h"
#include "XmlPageProcessor.h"

using std::string;
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

    TestDumpWriter writer;

    input.Process(handlers, &writer);
}