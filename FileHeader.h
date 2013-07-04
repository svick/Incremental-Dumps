#pragma once

#include <iostream>
#include "DumpObject.h"
#include "Offset.h"

using std::istream;

class FileHeader : public DumpObject
{
private:
    FileHeader(Offset fileEnd, Offset pageIdIndexRoot, Offset freeSpaceIndexRoot);
protected:
    virtual void WriteInternal(unique_ptr<iostream> const &stream);
public:
    static const char FileFormatVersion = 1;
    static const char FileDataVersion = 1;

    static FileHeader Read(unique_ptr<iostream> const &stream);

    Offset FileEnd;
    Offset PageIdIndexRoot;
    Offset FreeSpaceIndexRoot;

    FileHeader();
};