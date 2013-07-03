#include <string>
#include <iostream>
#include "TestDumpWriter.h"

using std::string;
using std::cout;

// http://stackoverflow.com/a/14678800/41071
string TestDumpWriter::ReplaceString(string subject, const string& search, const string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

void TestDumpWriter::WritePage(const shared_ptr<const Page> page)
{
    cout << page->Title << "\n";
}

void TestDumpWriter::WriteRevision(const shared_ptr<const Revision> revision)
{
    cout << " " << ReplaceString(revision->Text, "\n", "\\n").substr(0, 78) << "\n";
}