#include <iostream>
#include "XML/xmlinput.h"
#include "XML/xmlfile.h"
#include "DumpWriters/PagesHistoryWriter.h"
#include "XmlPageProcessor.h"
#include "Dump.h"
#include "DumpObjects/DumpRevision.h"
#include "Indexes/Index.h"

using std::cin;
using std::cout;

class StandardInputStream : public XML::InputStream
{
public:
    virtual int read(XML_Char *buf, size_t bufLen)
    {
        cin.read(buf, bufLen);
        return cin.gcount();
    }
};

void mediawikiHandler(XML::Element &elem, void *userData)
{
    XML::Handler handlers[] = {
        XML::Handler("page", XmlPageProcessor::Handler),
        XML::Handler::END
    };

    elem.Process(handlers, userData);
}

void printUsage()
{
    cout << "Usage:\n";
    cout << "creating dump: idumps c[reate] source.xml dump.id\n";
    cout << "reading dump: idumps r[ead] dump.id\n";
}

void createDump(string inputFileName, string outputFileName)
{
    XML::FileInputStream stream = XML::FileInputStream(inputFileName.c_str());

    XML::Input input(stream);

    XML::Handler handlers[] = {
        XML::Handler(mediawikiHandler),
        XML::Handler::END
    };

    shared_ptr<WritableDump> dump = WritableDump::Create(outputFileName);

    PagesHistoryWriter writer(dump);

    input.Process(handlers, &writer);

    dump->WriteIndexes();
}

void readDump(string dumpFileName)
{
    auto dump = WritableDump::Create(dumpFileName);

    int i = 0;
    for (auto pageInfo : *dump->pageIdIndex)
    {
        auto page = DumpPage(dump, pageInfo.second).page;

        cout << page.PageId << ": " << page.Title;

        if (page.RedirectTarget != string())
        {
            cout << " -> " << page.RedirectTarget;
        }

        cout << ", " << page.RevisionIds.size() << " revs\n";

        int j = 0;
        for (auto revisionId : page.RevisionIds)
        {
            auto revision = DumpRevision(dump, revisionId, false).revision;

            cout << " " << revision.RevisionId << " (<- " << revision.ParentId << ") " << revision.DateTime.ToString() << " " << revision.Contributor->UserName << "\n";
            cout << "  " << revision.Comment << "\n";

            if (revision.Text.length() > 0)
            {
                cout << "  " << revision.Text.substr(0, 77) << "\n";
            }

            if (++j >= 5)
                break;
        }

        if (++i >= 5)
            break;
    }
}

int main(int argc, const char* argv[])
{
    if (argc == 1)
    {
        printUsage();
        return 0;
    }

    string action = argv[1];

    if (action == "c" || action == "create")
    {
        if (argc != 4)
        {
            cout << "Invalid number of parameters\n";
            printUsage();
        }
        else
        {
            createDump(argv[2], argv[3]);
        }
    }
    else if (action == "r" || action == "read")
    {
        if (argc != 3)
        {
            cout << "Invalid number of parameters\n";
            printUsage();
        }
        else
        {
            readDump(argv[2]);
        }
    }
    else
    {
        cout << "Unknown action '" << action << "'\n";
        printUsage();
    }
}