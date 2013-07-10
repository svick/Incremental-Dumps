#pragma once

#include <memory>

#include "../Objects/Page.h"
#include "../Objects/Revision.h"

using std::shared_ptr;

class DumpWriter
{
public:
    virtual void StartPage(const shared_ptr<const Page> page) = 0;
    virtual void AddRevision(const shared_ptr<const Revision> revision) = 0;
    virtual void EndPage() = 0;
};