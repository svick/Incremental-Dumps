#pragma once

#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "../DumpException.h"

using std::uint32_t;
using std::numeric_limits;
using std::unique_ptr;
using std::string;
using std::istream;
using std::ostream;
using std::vector;

// TODO: add non-template wrapper with template methods, so that type inference can work?

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

    static uint32_t DumpSize()
    {
        return T::DumpSize();
    }

    static uint32_t DumpSize(const T value)
    {
        return DumpSize();
    }
};

template<>
class DumpTraits<uint32_t>
{
public:
    static uint32_t Read(istream &stream)
    {
        char bytes[4];

        stream.read(bytes, 4);

        // TODO: use loop and make generic
        uint32_t result = 0;
        result |= (uint32_t)(uint8_t)bytes[0];
        result |= (uint32_t)(uint8_t)bytes[1] << 8;
        result |= (uint32_t)(uint8_t)bytes[2] << 16;
        result |= (uint32_t)(uint8_t)bytes[3] << 24;

        return result;
    }

    static void Write(ostream &stream, const uint32_t value)
    {
        char bytes[4];

        bytes[0] = value & 0xFF;
        bytes[1] = (value >> 8) & 0xFF;
        bytes[2] = (value >> 16) & 0xFF;
        bytes[3] = (value >> 24) & 0xFF;

        stream.write(bytes, 4);
    }

    static uint32_t DumpSize(const uint32_t value = 0)
    {
        return 4;
    }
};

template<>
class DumpTraits<uint16_t>
{
public:
    static uint16_t Read(istream &stream)
    {
        char bytes[2];

        stream.read(bytes, 2);

        uint16_t result = 0;
        result |= (uint16_t)(uint8_t)bytes[0];
        result |= (uint16_t)(uint8_t)bytes[1] << 8;

        return result;
    }

    static void Write(ostream &stream, const uint16_t value)
    {
        char bytes[2];

        bytes[0] = value & 0xFF;
        bytes[1] = (value >> 8) & 0xFF;

        stream.write(bytes, 2);
    }

    static uint32_t DumpSize(const uint16_t value = 0)
    {
        return 2;
    }
};

template<>
class DumpTraits<uint8_t>
{
public:
    static uint8_t Read(istream &stream)
    {
        char byte;
        stream.read(&byte, 1);
        return byte;
    }

    static void Write(ostream &stream, const uint8_t value)
    {
        stream.put(value);
    }

    static uint32_t DumpSize(const uint8_t value = 0)
    {
        return 1;
    }
};

// for now, handle only strings of length up to 255
template<>
class DumpTraits<string>
{
private:
    static string ReadCore(istream &stream, uint32_t count)
    {
        auto bytes = unique_ptr<char[]>(new char[count]);
        stream.read(bytes.get(), count);

        return string(bytes.get(), count);
    }

public:
    static string Read(istream &stream)
    {
        uint8_t count = DumpTraits<uint8_t>::Read(stream);

        return ReadCore(stream, count);
    }

    static string ReadLong(istream &stream)
    {
        uint32_t count = DumpTraits<uint32_t>::Read(stream);

        return ReadCore(stream, count);
    }

    static void Write(ostream &stream, const string value)
    {
        auto length = value.length();

        if (length > 255)
        {
            // invalid UTF-8 at the end of a string is represented as U+FFFD
            // this can get string over 255 bytes, so that character needs to be removed

            string replacementChar = "\xEF\xBF\xBD"; // UTF-8 encoded U+FFFD REPLACEMENT CHARACTER
            if (value.substr(value.length() - 3) == replacementChar)
            {
                string fixedValue = value.substr(0, value.length() - 3);
                Write(stream, fixedValue);
            }
            else
                throw DumpException();
        }

        DumpTraits<uint8_t>::Write(stream, length);

        stream.write(value.data(), length);
    }

    static void WriteLong(ostream &stream, const string value)
    {
        auto length = value.length();

        if (length > 0xFFFFFFFF)
        {
            throw DumpException();
        }

        DumpTraits<uint32_t>::Write(stream, length);

        stream.write(value.data(), length);
    }

    static uint32_t DumpSize(const string value)
    {
        return DumpTraits<uint8_t>::DumpSize(value.length()) + value.length();
    }

    static uint32_t DumpSizeLong(const string value)
    {
        return DumpTraits<uint32_t>::DumpSize(value.length()) + value.length();
    }
};

template<typename T>
class DumpTraits<vector<T>>
{
public:
    static vector<T> Read(istream &stream)
    {
        uint32_t count = DumpTraits<uint32_t>::Read(stream);

        vector<T> result;

        for (uint32_t i = 0; i < count; i++)
        {
            result.push_back(DumpTraits<T>::Read(stream));
        }

        return result;
    }

    static void Write(ostream &stream, const vector<T> value)
    {
        auto length = value.size();

        if (length >= numeric_limits<uint32_t>::max())
            throw DumpException();

        DumpTraits<uint32_t>::Write(stream, length);

        for (T item : value)
        {
            DumpTraits<T>::Write(stream, item);
        }
    }

    static uint32_t DumpSize(const vector<T> value)
    {
        uint32_t size = DumpTraits<uint32_t>::DumpSize(value.size());

        for (T item : value)
        {
            size += DumpTraits<T>::DumpSize(item);
        }

        return size;
    }
};