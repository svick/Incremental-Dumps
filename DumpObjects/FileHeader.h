#pragma once

#include <iostream>
#include "DumpObject.h"
#include "Offset.h"

class ReadableDump;

using std::istream;

class FileHeader : public DumpObject
{
private:
    static uint32_t Length();

    ostream* stream;

    FileHeader(Offset fileEnd, Offset pageIdIndexRoot, Offset revisionIdIndexRoot, Offset freeSpaceIndexRoot, weak_ptr<WritableDump> dump = weak_ptr<WritableDump>());
protected:
    void WriteInternal();
public:
    static const uint8_t FileFormatVersion = 1;
    static const uint8_t FileDataVersion = 1;

    static FileHeader Read(ReadableDump const &dump);

    virtual void Write() override;
    virtual uint32_t NewLength() override;

    Offset FileEnd;
    Offset PageIdIndexRoot;
    Offset RevisionIdIndexRoot;
    Offset FreeSpaceIndexRoot;

    FileHeader(weak_ptr<WritableDump> dump = weak_ptr<WritableDump>());
};