#include <iostream>
#include <fstream>
#include <queue>
#include "DumpWriters/CompositeWriter.h"
#include "DumpWriters/DumpWriter.h"
#include "DumpWriters/CurrentWriterWrapper.h"
#include "DumpWriters/ArticlesWriterWrapper.h"
#include "DumpWriters/ProgressWriterWrapper.h"
#include "Diff/DiffReader.h"
#include "XmlInput/XmlMediawikiProcessor.h"
#include "XmlInput/WrapperInputStream.h"
#include "XmlWriter.h"
#include "Dump.h"
#include "FetchText.h"
#include "StringHelpers.h"
#include "format.h"

/** @file */

void printUsage()
{
    std::cout << "Usage:\n";
    std::cout << "reading dump: idumps r[ead] dump.id output.xml\n";
    std::cout << "applying diff: idumps a[pply] dump.id diff.dd\n";
    std::cout << "updating dump: idumps u[pdate] name new-timestamp phpPath dumpBackup fetchText spec dump.id ...\n";
    std::cout << " name is the name of the wiki (e.g. enwiki)\n";
    std::cout << " timestamp identifies this specific dump (it doesn't actually have to be a timestamp)\n";
    std::cout << " spec is a 2 to 4-letter string that describes what kind of dump to create:\n";
    std::cout << " 1. letter: p for pages dump or s for stub dump\n";
    std::cout << " 2. letter: h for history dump or c for current dump\n";
    std::cout << " 3. optional letter: a for articles dump\n";
    std::cout << " 4. optional letter: d to also create diff dump\n";
    std::cout << "  add the path to the diff dump after the path to dump\n";
    std::cout << " example: sh for stub-meta-history, pcad for pages-articles with diff dump\n";
    std::cout << "creating dump: idumps c[reate] name timestamp source.xml spec dump.id ...\n";
}

/**
 * Creates an IDumpWriter from command line parameters.
 *
 * @param parameters Collection of remaining command line parameters. This method consumes the parameters it needs from it.
 * @param name Name of the dump
 * @param timestamp Timestamp of the dump
 */
std::unique_ptr<IDumpWriter> createWriter(std::queue<std::string> &parameters, const std::string &name, const std::string &timestamp)
{
    if (parameters.size() < 2)
        throw ParametersException("Incorrect number of parameters.");

    auto spec = parameters.front();
    parameters.pop();

    if (spec.length() < 2 || spec.length() > 4)
        throw ParametersException(str(fmt::Format("{0} is an invalid dump spec (wrong length).") << spec));

    auto fileName = parameters.front();
    parameters.pop();

    bool withText;
    if (spec.at(0) == 'p')
        withText = true;
    else if (spec.at(0) == 's')
        withText = false;
    else
        throw ParametersException(str(fmt::Format("The first letter of spec has to be p (for pages) or s (for stub), not {0}.") << spec.at(0)));

    bool current;
    if (spec.at(1) == 'c')
        current = true;
    else if (spec.at(1) == 'h')
        current = false;
    else
        throw ParametersException(str(fmt::Format("The second letter of spec has to be c (for current) or h (for history), not {0}.") << spec.at(1)));

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
            throw ParametersException(str(fmt::Format("The letter {0} is not valid at this point in the spec.") << spec.at(i)));
    }

    auto dump = WritableDump::Create(fileName);

    dump->siteInfo->CheckName(name, true);
    dump->siteInfo->name = name;

    dump->siteInfo->CheckTimestamp(timestamp, true);
    auto oldTimestamp = dump->siteInfo->timestamp;
    dump->siteInfo->timestamp = timestamp;

    std::unique_ptr<DiffWriter> diffWriter;
    if (diffDump)
    {
        if (parameters.empty())
            throw ParametersException("The filename of the diff dump is missing.");

        auto diffFileName = parameters.front();
        parameters.pop();

        diffWriter = std::unique_ptr<DiffWriter>(new DiffWriter(diffFileName, name, oldTimestamp, timestamp));
    }

    auto writer = std::unique_ptr<IDumpWriter>(new DumpWriter(dump, withText, std::move(diffWriter)));

    if (current)
        writer = std::unique_ptr<IDumpWriter>(new CurrentWriterWrapper(std::move(writer)));

    if (articles)
        writer = std::unique_ptr<IDumpWriter>(new ArticlesWriterWrapper(std::move(writer)));

    return writer;
}

/**
 * Creates a collection of IDumpWriter from command line parameters.
 *
 * @param parameters Collection of remaining command line parameters. This method tries to consume all the parameters in it.
 * @param name Name of the dump
 * @param timestamp Timestamp of the dump
 */
std::vector<std::unique_ptr<IDumpWriter>> createWriters(std::queue<std::string> &parameters, const std::string &name, const std::string &timestamp)
{
    std::vector<std::unique_ptr<IDumpWriter>> writers;

    while (!parameters.empty())
    {
        auto writer = createWriter(parameters, name, timestamp);

        writers.push_back(std::move(writer));
    }

    return writers;
}

/**
 *  Reads and verifies dump name and timestamp parameters from the parameters collection.
 *
 * @param parameters Collection of remaining command line parameters. This method consumes two parameters from it.
 */
void readNameAndTimestamp(std::queue<std::string> &parameters, std::string &name, std::string &timestamp)
{
    name = parameters.front();
    parameters.pop();

    if (name.empty())
        throw ParametersException("The name of the dump can't be empty.");

    timestamp = parameters.front();
    parameters.pop();

    if (timestamp.empty())
        throw ParametersException("The timestamp can't be empty.");
}

/**
 * Helper function for createDump().
 *
 * Reads XML from @a inputStream and writes it to @a writer.
 */
void createDumpCore(ProgressWriterWrapper& writer, std::istream& inputStream)
{
    std::uint64_t i = 0;
    std::uint64_t totalRead = 0;
    std::function<void(int)> offsetReportingFunction = [&](int read)
    {
        totalRead += read;

        if (i % 100 == 0)
            writer.ReportOffset(totalRead);

        i++;
    };

    writer.SetDumpKind(DumpKind::None);

    WrapperInputStream wrapperStream(inputStream, offsetReportingFunction);

    XmlMediawikiProcessor::Process(&writer, wrapperStream);

    writer.Complete();
}

/**
 * Creates or updates incremental dump (or dumps) from XML dump.
 *
 * @param parameters Collection of command line parameters
 */
void createDump(std::queue<std::string> &parameters)
{
    if (parameters.size() < 3 + 2)
        throw ParametersException("Not enough parameters.");

    std::uint32_t reportPeriod = 10000;

    if (parameters.front() == "--report")
    {
        parameters.pop();

        bool success;
        long parsed = tryParseLong(parameters.front(), success);

        if (!success || parsed < 0 || parsed > 0xFFFFFFFF)
        {
            throw UserException(
                str(fmt::Format("The value {0} is not valid report period.") << parameters.front()));
        }

        reportPeriod = parsed;
        parameters.pop();
    }

    if (parameters.size() < 3 + 2)
        throw ParametersException("Not enough parameters.");

    std::string name, timestamp;
    readNameAndTimestamp(parameters, name, timestamp);

    std::string inputFileName = parameters.front();
    parameters.pop();

    auto writers = createWriters(parameters, name, timestamp);

    std::unique_ptr<CompositeWriter> writer(new CompositeWriter(std::move(writers)));

    ProgressWriterWrapper progressWriter(std::move(writer), reportPeriod);

    if (inputFileName == "-")
    {
        std::cin.exceptions(std::ios::badbit);
        createDumpCore(progressWriter, std::cin);
    }
    else
    {
        std::ifstream stream(inputFileName, std::ios::binary);
        stream.exceptions(std::ios::badbit);
        createDumpCore(progressWriter, stream);
    }
}

/**
 * Creates or updates incremental dumps from MediaWiki.
 *
 * @param parameters Collection of command line parameters
 */
void updateDump(std::queue<std::string> &parameters)
{
    if (parameters.size() < 5 + 2)
        throw ParametersException("Not enough parameters.");

    std::string name, timestamp;
    readNameAndTimestamp(parameters, name, timestamp);

    std::string phpPath = parameters.front();
    parameters.pop();

    std::string dumpBackupParameters = parameters.front();
    parameters.pop();

    std::string fetchTextParameters = parameters.front();
    parameters.pop();

    auto writers = createWriters(parameters, name, timestamp);

    FetchText fetchText(std::unique_ptr<exec_stream_t>(new exec_stream_t(phpPath, fetchTextParameters)));

    CompositeWriter writer(std::move(writers), [&](int textId) { return fetchText.GetText(textId); });

    writer.SetDumpKind(DumpKind::None);

    exec_stream_t dumpBackupProcess;
    dumpBackupProcess.set_buffer_limit(exec_stream_t::s_out, 8192);
    dumpBackupProcess.start(phpPath, dumpBackupParameters + " --full --stub");

    std::function<void()> progressForwardingFunction = [&]()
        {
            auto &stream = dumpBackupProcess.err();
            char buffer[1024];
            std::streamsize count = 0;
            do
            {
                count = stream.readsome(buffer, 1024);
                std::cerr.write(buffer, count);
            } while (count != 0);
        };
    WrapperInputStream dumpBackupStream(dumpBackupProcess.out(), progressForwardingFunction);

    XmlMediawikiProcessor::Process(&writer, dumpBackupStream);

    writer.Complete();
}

/**
 * Reads incremental dump and writes its contents as a XML dump file.
 *
 * @param dumpFileName Path to the incremental dump file to be read
 * @param outputFileName Path to the XML dump file to be written
 */
void readDump(std::string dumpFileName, std::string outputFileName)
{
    auto dump = WritableDump::Create(dumpFileName);

    XmlWriter::WriteDump(dump, outputFileName);
}

/**
 * Applies diff dump to incremental dump.
 *
 * @param dumpFileName Path to the incremental dump file to be updated
 * @param diffFileName Path to the diff dump file
 */
void applyDiff(std::string dumpFileName, std::string diffFileName)
{
    auto dump = WritableDump::Create(dumpFileName);

    ChangeProcessor changeProcessor(dump);
    DiffReader diffReader(diffFileName, changeProcessor);
    diffReader.Read();
}

#ifdef __GNUC__
#include <execinfo.h>

/**
 * Function for better reporting of unhandled exceptions in GCC.
 *
 * Code from http://stackoverflow.com/a/18878080/41071.
 */
void terminate_handler()
{
    void** buffer = new void*[50];
    int count = backtrace(buffer, 50);
    backtrace_symbols_fd(buffer, count, STDERR_FILENO);

    auto ptr = std::current_exception();
    try
    {
        std::rethrow_exception(ptr);
    }
    catch (std::exception& p)
    {
        std::cerr << p.what() << "\n";
    }
}
#endif

int main(int argc, const char* argv[])
{
#ifdef __GNUC__
    std::set_terminate(terminate_handler);
#endif

    if (argc == 1)
    {
        printUsage();
        return 0;
    }

    std::vector<std::string> args(argv, argv + argc);

    string action = args.at(1);

    try
    {
        if (action == "c" || action == "create")
        {
            std::queue<std::string> parameters;

            for (size_t i = 2; i < args.size(); i++)
                parameters.push(args.at(i));

            createDump(parameters);
        }
        else if (action == "u" || action == "update")
        {
            std::queue<std::string> parameters;

            for (size_t i = 2; i < args.size(); i++)
                parameters.push(args.at(i));

            updateDump(parameters);
        }
        else if (action == "r" || action == "read")
        {
            if (argc != 4)
                throw ParametersException("Invalid number of parameters.");

            readDump(args.at(2), args.at(3));
        }
        else if (action == "a" || action == "apply")
        {
            if (argc != 4)
                throw ParametersException("Invalid number of parameters.");

            applyDiff(args.at(2), args.at(3));
        }
        else
        {
            throw ParametersException(str(fmt::Format("Unknown action {0}.") << action));
        }
    }
    catch (ParametersException &ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        printUsage();
    }
    catch (UserException &ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
    }
}
