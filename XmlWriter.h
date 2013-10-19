#pragma once

#include <memory>
#include <string>
#include "Dump.h"

/**
 * Used for writing XML from data in incremental dump.
 */
class XmlWriter
{
private:
    XmlWriter()
    {}
public:
    /**
     * Reads incremental dump and writes its contents to a XML file.
     */
    static void WriteDump(std::shared_ptr<Dump> dump, std::string fileName);
};