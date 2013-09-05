#pragma once

#include <cstdint>
#include <memory>
#include <iostream>
#include "Offset.h"
#include "DumpTraits.h"

class WritableDump;

class DumpObjectBase
{
protected:
    std::ostream *stream;

    virtual void WriteInternal() = 0;

    template<typename T>
    void WriteValue(const T &value);
public:
    template<typename T>
    static void WriteValue(std::ostream &stream, const T &value);

    template<typename T>
    static void ReadValue(std::istream &stream, T &value);

    template<typename T>
    static std::uint32_t ValueSize(const T &value);

    virtual std::uint32_t NewLength() = 0;

    virtual ~DumpObjectBase() {}
};

class DumpObject : public DumpObjectBase
{
protected:
    std::weak_ptr<WritableDump> dump;
    std::uint64_t savedOffset;
    std::uint32_t savedLength;

    DumpObject(std::weak_ptr<WritableDump> dump);
    virtual void WriteInternal() override = 0;
    virtual void UpdateIndex(Offset offset, bool overwrite);
public:
    virtual void Write();
    virtual std::uint32_t NewLength() override = 0;
    std::uint64_t SavedOffset() const;
};

template<typename T>
void DumpObjectBase::WriteValue(const T &value)
{
    WriteValue(*stream, value);
}

template<typename T>
void DumpObjectBase::WriteValue(std::ostream &stream, const T &value)
{
    DumpTraits<T>::Write(stream, value);
}

template<typename T>
void DumpObjectBase::ReadValue(std::istream &stream, T &value)
{
    value = DumpTraits<T>::Read(stream);
}

template<typename T>
uint32_t DumpObjectBase::ValueSize(const T &value)
{
    return DumpTraits<T>::DumpSize(value);
}