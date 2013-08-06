#pragma once

#include "../DumpWriters/DumpWriter.h"
#include "../XML/xmlinput.h"

class XmlMediawikiProcessor
{
private:
    XmlMediawikiProcessor()
    {}
public:
    static void Process(IDumpWriter *writer, XML::InputStream &inputStream);
    static void Process(IDumpWriter *writer, std::string inputFileName);
};