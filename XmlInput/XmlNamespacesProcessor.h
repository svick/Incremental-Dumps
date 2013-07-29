#pragma once

#include "../XML/xmlinput.h"

class XmlNamespacesProcessor
{
private:
    XmlNamespacesProcessor()
    {}
public:
    static void Handler(XML::Element &elem, void *userData);
};