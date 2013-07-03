#pragma once

#include <string>
#include "DumpWriter.h"

using std::string;

class TestDumpWriter : public DumpWriter
{
private:
    string ReplaceString(string subject, const string& search, const string& replace);
public:
    virtual void WritePage(const shared_ptr<Page const> page);
    virtual void WriteRevision(const shared_ptr<const Revision> revision);
};