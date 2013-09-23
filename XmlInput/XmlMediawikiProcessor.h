#pragma once

#include "../DumpWriters/DumpWriter.h"
#include "../XML/xmlinput.h"

/**
 * The main class for reading XML dumps.
 */
class XmlMediawikiProcessor
{
private:
    XmlMediawikiProcessor()
    {}

    /**
     * Handler for the @c @<mediawiki@> element.
     *
     * @param userData Expects this to be a pointer to IDumpWriter.
     */
    static void Handler(XML::Element &elem, void *userData);
public:
    /**
     * Reads XML from the input stream and writes it to the given dump writer.
     */
    static void Process(IDumpWriter *writer, XML::InputStream &inputStream);

    /**
     * Reads XML from the input file and writes it to the given dump writer.
     */
    static void Process(IDumpWriter *writer, std::string inputFileName);
};