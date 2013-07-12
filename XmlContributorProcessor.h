#pragma once

#include <cstdint>
#include <string>
#include "XML/xmlinput.h"

using std::uint32_t;
using std::string;

class XmlContributorProcessor
{
private:
    string ip;
    string userName;
    uint32_t id;
public:
     static void Handler(XML::Element &elem, void *userData);
};