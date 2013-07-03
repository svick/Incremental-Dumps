#pragma once

#include "XML/xmlinput.h"
#include "Revision.h"

class XmlRevisionProcessor
{
private:
    static void textHandler(XML::Element &elem, void *userData);
public:
    static int revisionIndex;

    static void Handler(XML::Element &elem, void *userData);
};