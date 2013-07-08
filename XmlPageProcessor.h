#pragma once

#include <memory>
#include "XML/xmlinput.h"
#include "DumpObjects/Page.h"
#include "DumpWriter.h"

using std::shared_ptr;

class XmlPageProcessor
{
private:
    shared_ptr<Page> page;
    DumpWriter* dumpWriter;
    bool pageWritten;

    XmlPageProcessor(const shared_ptr<Page> page, DumpWriter* dumpWriter);

    static void titleHandler(XML::Element &elem, void *userData);
    void writePage();
public:
    static void Handler(XML::Element &elem, void *userData);
    void ProcessRevision(const shared_ptr<const Revision> revision);
};