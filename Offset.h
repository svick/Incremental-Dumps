#pragma once

#include <cstdint>
#include <memory>
#include <iostream>

using std::int64_t;
using std::unique_ptr;
using std::iostream;

class Offset
{
public:
    int64_t value;

    Offset(int64_t value);
    void Write(unique_ptr<iostream> const &stream) const;
    static Offset Read(unique_ptr<iostream> const &stream);
};