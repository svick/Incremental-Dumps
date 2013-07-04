#include "DumpObject.h"

void DumpObject::Write(unique_ptr<iostream> const &stream, int64_t offset)
{
    stream->seekp(offset);
    WriteInternal(stream);
}