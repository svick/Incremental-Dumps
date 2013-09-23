#pragma once

#include "../XML/xmlinput.h"

/**
 * Processes the @c @<revision@> element.
 */
class XmlRevisionProcessor
{
public:
    /**
     * Handler for the @c @<revision@> element.
     *
     * @param userData Expects this to be a pointer to XmlPageProcessor.
     */
    static void Handler(XML::Element &elem, void *userData);
};