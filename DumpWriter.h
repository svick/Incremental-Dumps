#pragma once

#include <memory>

#include "DumpObjects/Page.h"
#include "DumpObjects/Revision.h"

using std::shared_ptr;

class DumpWriter
{
public:
    virtual void WritePage(const shared_ptr<const Page> page) = 0;
    virtual void WriteRevision(const shared_ptr<const Revision> revision) = 0;
};