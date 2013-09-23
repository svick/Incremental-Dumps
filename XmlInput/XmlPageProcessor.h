#pragma once

#include <memory>
#include "../XML/xmlinput.h"
#include "../Objects/Page.h"
#include "../DumpWriters/DumpWriter.h"

/**
 * Processes the @c @<page@> element.
 */
class XmlPageProcessor
{
private:
    IDumpWriter* dumpWriter;
    bool pageWritten;

    std::shared_ptr<Page> page;

    XmlPageProcessor(const shared_ptr<Page> page, IDumpWriter* dumpWriter);

    void writePage();
    void completePage();
public:
    /**
     * Handler for the @c @<page@> element.
     *
     * @param userData Expects this to be a pointer to IDumpWriter.
     */
    static void Handler(XML::Element &elem, void *userData);

    /**
     * Resends the given revision to the IDumpWriter.
     *
     * Called from XmlRevisionProcessor.
     */
    void ProcessRevision(const shared_ptr<const Revision> revision);
};