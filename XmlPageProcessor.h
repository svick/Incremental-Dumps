#pragma once

#include <memory>
#include "XML/xmlinput.h"
#include "Objects/Page.h"
#include "DumpWriters/DumpWriter.h"

using std::shared_ptr;

class XmlPageProcessor
{
private:
    shared_ptr<Page> page;
    DumpWriter* dumpWriter;
    bool pageWritten;

    XmlPageProcessor(const shared_ptr<Page> page, DumpWriter* dumpWriter);

    void writePage();
    void completePage();
public:
    static void Handler(XML::Element &elem, void *userData);
    void ProcessRevision(const shared_ptr<const Revision> revision);
};