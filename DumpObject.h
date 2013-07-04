#pragma once

#include <cstdint>
#include <memory>
#include <iostream>

using std::int64_t;
using std::unique_ptr;
using std::iostream;

class DumpObject
{
protected:
    virtual void WriteInternal(unique_ptr<iostream> const &stream) = 0;
public:
    void Write(unique_ptr<iostream> const &stream, int64_t offset);
};