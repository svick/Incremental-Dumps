#pragma once

#include <string>
#include "DumpWriter.h"

using std::string;

class TestDumpWriter : public DumpWriter
{
private:
    string ReplaceString(string subject, const string& search, const string& replace);
public:
    virtual void StartPage(const shared_ptr<const Page> page);
    virtual void AddRevision(const shared_ptr<const Revision> revision);
    virtual void EndPage();
};