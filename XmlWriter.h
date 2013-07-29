#pragma once

#include <memory>
#include <string>
#include "Dump.h"

class XmlWriter
{
private:
    XmlWriter()
    {}
public:
    static void WriteDump(std::shared_ptr<WritableDump> dump, std::string fileName);
};