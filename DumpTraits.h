#pragma once

#include <cstdint>
#include <iostream>

using std::int32_t;
using std::istream;
using std::ostream;

template<typename T>
class DumpTraits
{
public:
    static T Read(istream &stream)
    {
        return T::Read(stream);
    }

    static void Write(ostream &stream, const T value)
    {
        value.Write(stream);
    }

    static int64_t DumpSize()
    {
        return T::DumpSize();
    }
};

template<>
class DumpTraits<int32_t>
{
public:
    static int32_t Read(istream &stream)
    {
        char bytes[4];

        stream.read(bytes, 4);

        int32_t result = 0;
        result |= (int64_t)bytes[0];
        result |= (int64_t)bytes[1] << 8;
        result |= (int64_t)bytes[2] << 16;
        result |= (int64_t)bytes[3] << 24;

        return result;
    }

    static void Write(ostream &stream, const int32_t value)
    {
        char bytes[4];

        bytes[0] = value & 0xFF;
        bytes[1] = (value >> 8) & 0xFF;
        bytes[2] = (value >> 16) & 0xFF;
        bytes[3] = (value >> 24) & 0xFF;

        stream.write(bytes, 4);
    }

    static int64_t DumpSize()
    {
        return 4;
    }
};

template<>
class DumpTraits<char>
{
public:
    static char Read(istream &stream)
    {
        char byte;
        stream.read(&byte, 1);
        return byte;
    }

    static void Write(ostream &stream, const char value)
    {
        stream.write(&value, 1);
    }

    static int64_t DumpSize()
    {
        return 1;
    }
};