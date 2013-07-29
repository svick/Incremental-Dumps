#include <iostream>
#include "DumpWriters/PagesHistoryWriter.h"
#include "XmlInput/XmlMediawikiProcessor.h"
#include "XmlWriter.h"
#include "Dump.h"

void printUsage()
{
    std::cout << "Usage:\n";
    std::cout << "creating dump: idumps c[reate] source.xml dump.id\n";
    std::cout << "reading dump: idumps r[ead] dump.id output.xml\n";
}

void createDump(string inputFileName, string outputFileName)
{
    auto dump = WritableDump::Create(outputFileName);

    PagesHistoryWriter writer(dump);

    XmlMediawikiProcessor::Process(&writer, inputFileName);

    dump->WriteIndexes();
}

void readDump(string dumpFileName, string outputFileName)
{
    auto dump = WritableDump::Create(dumpFileName);

    XmlWriter::WriteDump(dump, outputFileName);
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
            std::cout << "Invalid number of parameters\n";
            printUsage();
        }
        else
        {
            createDump(argv[2], argv[3]);
        }
    }
    else if (action == "r" || action == "read")
    {
        if (argc != 4)
        {
            std::cout << "Invalid number of parameters\n";
            printUsage();
        }
        else
        {
            readDump(argv[2], argv[3]);
        }
    }
    else
    {
        std::cout << "Unknown action '" << action << "'\n";
        printUsage();
    }
}