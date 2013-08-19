#include <iostream>
#include <queue>
#include "DumpWriters/CompositeWriter.h"
#include "DumpWriters/DumpWriter.h"
#include "DumpWriters/CurrentWriterWrapper.h"
#include "DumpWriters/ArticlesWriterWrapper.h"
#include "Diff/DiffReader.h"
#include "XmlInput/XmlMediawikiProcessor.h"
#include "XmlInput/WrapperInputStream.h"
#include "XmlWriter.h"
#include "Dump.h"
#include "FetchText.h"

void printUsage()
{
    std::cout << "Usage:\n";
    std::cout << "creating dump: idumps c[reate] source.xml spec dump.id ...\n";
    std::cout << " spec is a 2 to 4-letter string that describes what kind of dump to create:\n";
    std::cout << " 1. letter: p for pages dump or s for stub dump:\n";
    std::cout << " 2. letter: h for history dump or c for current dump:\n";
    std::cout << " 3. optional letter: a for articles dump:\n";
    std::cout << " 4. optional letter: d to also create diff dump:\n";
    std::cout << "  add the path to the diff dump after the path to dump\n";
    std::cout << " example: sh for stub-meta-history, pcad for pages-articles with diff dump\n";
    std::cout << "updating dump: idumps u[pdate] phpPath dumpBackup fetchText spec dump.id ...\n";
    std::cout << "reading dump: idumps r[ead] dump.id output.xml\n";
    std::cout << "applying diff: idumps a[pply] dump.id diff.dd\n";
}

std::unique_ptr<IDumpWriter> createWriter(std::queue<std::string> &parameters)
{
    std::unique_ptr<IDumpWriter> nullResult;

    if (parameters.size() < 2)
        return nullResult;

    auto spec = parameters.front();
    parameters.pop();

    if (spec.length() < 2 || spec.length() > 3)
        return nullResult;

    auto fileName = parameters.front();
    parameters.pop();

    bool withText;
    if (spec.at(0) == 'p')
        withText = true;
    else if (spec.at(0) == 's')
        withText = false;
    else
        return nullResult;

    bool current;
    if (spec.at(1) == 'c')
        current = true;
    else if (spec.at(1) == 'h')
        current = false;
    else
        return nullResult;

    bool articles = false;
    bool diffDump = false;

    if (spec.length() >= 3)
    {
        size_t i = 2;

        if (spec.at(i) == 'a')
        {
            articles = true;
            i++;
        }

        if (i < spec.length() && spec.at(i) == 'd')
        {
            diffDump = true;
            i++;
        }

        if (i != spec.length())
            return nullResult;
    }

    auto dump = WritableDump::Create(fileName);

    std::unique_ptr<DiffWriter> diffWriter;
    if (diffDump)
    {
        if (parameters.empty())
            return nullResult;

        auto diffFileName = parameters.front();
        parameters.pop();

        diffWriter = std::unique_ptr<DiffWriter>(new DiffWriter(diffFileName));
    }

    auto writer = std::unique_ptr<IDumpWriter>(new DumpWriter(dump, withText, std::move(diffWriter)));

    if (current)
        writer = std::unique_ptr<IDumpWriter>(new CurrentWriterWrapper(std::move(writer)));

    if (articles)
        writer = std::unique_ptr<IDumpWriter>(new ArticlesWriterWrapper(std::move(writer)));

    return writer;
}

std::vector<std::unique_ptr<IDumpWriter>> createWriters(std::queue<std::string> &parameters)
{
    std::vector<std::unique_ptr<IDumpWriter>> writers;

    while (!parameters.empty())
    {
        auto writer = createWriter(parameters);

        if (writer == nullptr)
            return std::vector<std::unique_ptr<IDumpWriter>>();

        writers.push_back(std::move(writer));
    }

    return writers;
}

bool createDump(std::queue<std::string> &parameters)
{
    std::string inputFileName = parameters.front();
    parameters.pop();

    auto writers = createWriters(parameters);

    if (writers.empty())
        return false;

    CompositeWriter writer(writers);

    writer.SetDumpKind(DumpKind::None);

    XmlMediawikiProcessor::Process(&writer, inputFileName);

    writer.EndDump();

    return true;
}

bool updateDump(std::queue<std::string> &parameters)
{
    std::string phpPath = parameters.front();
    parameters.pop();

    std::string dumpBackupParameters = parameters.front();
    parameters.pop();

    std::string fetchTextParameters = parameters.front();
    parameters.pop();

    auto writers = createWriters(parameters);

    if (writers.empty())
        return false;

    FetchText fetchText(std::unique_ptr<exec_stream_t>(new exec_stream_t(phpPath, fetchTextParameters)));

    CompositeWriter writer(writers, [&](int textId) { return fetchText.GetText(textId); });

    writer.SetDumpKind(DumpKind::None);

    exec_stream_t dumpBackupProcess;
    dumpBackupProcess.set_buffer_limit(exec_stream_t::s_out, 8192);
    dumpBackupProcess.start(phpPath, dumpBackupParameters);

    WrapperInputStream dumpBackupStream(dumpBackupProcess.out(),
        [&]()
        {
            auto &stream = dumpBackupProcess.err();
            char buffer[1024];
            std::streamsize count = 0;
            do
            {
                count = stream.readsome(buffer, 1024);
                std::cerr.write(buffer, count);
            } while (count != 0);
        });

    XmlMediawikiProcessor::Process(&writer, dumpBackupStream);

    writer.EndDump();

    return true;
}

void readDump(std::string dumpFileName, std::string outputFileName)
{
    auto dump = WritableDump::Create(dumpFileName);

    XmlWriter::WriteDump(dump, outputFileName);
}

void applyDiff(std::string dumpFileName, std::string diffFileName)
{
    auto dump = WritableDump::Create(dumpFileName);

    ChangeProcessor changeProcessor(dump);
    DiffReader diffReader(diffFileName, changeProcessor);
    diffReader.Read();
}

int main(int argc, const char* argv[])
{
    if (argc == 1)
    {
        printUsage();
        return 0;
    }

    std::vector<std::string> args(argv, argv + argc);

    string action = args.at(1);

    if (action == "c" || action == "create")
    {
        std::queue<std::string> parameters;

        for (size_t i = 2; i < args.size(); i++)
            parameters.push(args.at(i));

        if (!createDump(parameters))
        {
            std::cout << "Invalid parameters\n";
            printUsage();
        }
    }
    else if (action == "u" || action == "update")
    {
        std::queue<std::string> parameters;

        for (size_t i = 2; i < args.size(); i++)
            parameters.push(args.at(i));

        if (!updateDump(parameters))
        {
            std::cout << "Invalid parameters\n";
            printUsage();
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
            readDump(args.at(2), args.at(3));
        }
    }
    else if (action == "a" || action == "apply")
    {
        if (argc != 4)
        {
            std::cout << "Invalid number of parameters\n";
            printUsage();
        }
        else
        {
            applyDiff(args.at(2), args.at(3));
        }
    }
    else
    {
        std::cout << "Unknown action '" << action << "'\n";
        printUsage();
    }
}