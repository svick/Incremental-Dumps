#pragma once

#include "XML/xmlinput.h"

class XmlRevisionProcessor
{
public:
    static void Handler(XML::Element &elem, void *userData);
};