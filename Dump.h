#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <iostream>
#include "FileHeader.h"

using std::int64_t;
using std::unique_ptr;
using std::string;
using std::iostream;

class DumpException : std::exception
{
};

class ReadableDump
{
protected:
    unique_ptr<iostream> stream;
    ReadableDump(unique_ptr<iostream> stream);
public:
    ReadableDump(string fileName);
};

class WritableDump : public ReadableDump
{
private:
    FileHeader fileHeader;

    static unique_ptr<iostream> openStream(string fileName);
public:
    WritableDump(string fileName);
};