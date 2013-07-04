#include <cstdint>
#include <memory>
#include <string>
#include <fstream>
#include "Dump.h"

using std::unique_ptr;
using std::move;
using std::string;
using std::fstream;
using std::ios;

ReadableDump::ReadableDump(unique_ptr<iostream> stream)
    : stream(move(stream))
{}

ReadableDump::ReadableDump(string fileName)
    : stream(unique_ptr<fstream>(new fstream(fileName, ios::in | ios::binary)))
{}

unique_ptr<iostream> WritableDump::openStream(string fileName)
{
    fstream *stream = new fstream(fileName, ios::in | ios::out | ios::binary);

    if (!stream->is_open())
    {
        // this feels dangerous, isn't there a better way?
        stream = new fstream(fileName, ios::in | ios::out | ios::binary | ios::trunc);
    }

    stream->exceptions(ios::failbit | ios::badbit);

    return unique_ptr<iostream>(stream);
}

WritableDump::WritableDump(string fileName)
    : ReadableDump(openStream(fileName))
{
    if (stream->peek() == EOF)
    {
        stream->clear();
        fileHeader = FileHeader();
        fileHeader.Write(stream, 0);
    }
    else
    {
        fileHeader = FileHeader::Read(stream);
    }
}