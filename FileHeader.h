#pragma once

#include <iostream>
#include "DumpObject.h"
#include "Offset.h"

class ReadableDump;

using std::istream;

class FileHeader : public DumpObject
{
private:
    FileHeader(Offset fileEnd, Offset pageIdIndexRoot, Offset freeSpaceIndexRoot, weak_ptr<WritableDump> dump = weak_ptr<WritableDump>());
protected:
    virtual void Write(ostream &stream);
public:
    static const char FileFormatVersion = 1;
    static const char FileDataVersion = 1;

    static FileHeader Read(ReadableDump const &dump);

    virtual void Write();
    virtual int64_t NewLength();

    Offset FileEnd;
    Offset PageIdIndexRoot;
    Offset FreeSpaceIndexRoot;

    FileHeader(weak_ptr<WritableDump> dump = weak_ptr<WritableDump>());
};