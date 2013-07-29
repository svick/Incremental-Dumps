#pragma once

#include "../DumpWriters/DumpWriter.h"
#include "../XML/xmlinput.h"

class XmlMediawikiProcessor
{
private:
    XmlMediawikiProcessor()
    {}
public:
    static void Process(DumpWriter *writer, std::string inputFileName);
};