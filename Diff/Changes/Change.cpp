#include "Change.h"

void Change::Write(std::ostream *stream)
{
    this->stream = stream;
    WriteInternal();
    this->stream = nullptr;
}