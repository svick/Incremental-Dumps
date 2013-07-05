#pragma once

#include <cstdint>
#include <memory>
#include <iostream>

class WritableDump;

using std::int64_t;
using std::unique_ptr;
using std::weak_ptr;
using std::ostream;

class DumpObject
{
protected:
    weak_ptr<WritableDump> dump;
    int64_t savedOffset;
    int64_t savedLength;

    DumpObject(weak_ptr<WritableDump> dump);
    virtual void Write(ostream &stream) = 0;

public:
    virtual void Write();
    virtual int64_t NewLength() = 0;
    int64_t SavedOffset();
};