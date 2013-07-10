#pragma once

#include <iostream>
#include "DumpObject.h"
#include "Offset.h"

class ReadableDump;

using std::istream;

class FileHeader : public DumpObject
{
private:
    ostream* stream;

    FileHeader(Offset fileEnd, Offset pageIdIndexRoot, Offset freeSpaceIndexRoot, weak_ptr<WritableDump> dump = weak_ptr<WritableDump>());
protected:
    void WriteInternal();
public:
    static const uint8_t FileFormatVersion = 1;
    static const uint8_t FileDataVersion = 1;

    static FileHeader Read(ReadableDump const &dump);

    virtual void Write();
    virtual uint32_t NewLength() const;

    Offset FileEnd;
    Offset PageIdIndexRoot;
    Offset FreeSpaceIndexRoot;

    FileHeader(weak_ptr<WritableDump> dump = weak_ptr<WritableDump>());
};