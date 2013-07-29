#pragma once

#include <cstdint>
#include <memory>
#include <iostream>
#include "Offset.h"
#include "DumpTraits.h"

class WritableDump;

using std::uint64_t;
using std::unique_ptr;
using std::weak_ptr;
using std::ostream;

class DumpObjectBase
{
protected:
    ostream *stream;

    virtual void WriteInternal() = 0;

    template<typename T>
    void WriteValue(const T value);

    template<typename T>
    static void ReadValue(std::istream &stream, T &value);

    template<typename T>
    uint32_t ValueSize(const T value) const;
public:
    virtual uint32_t NewLength() = 0;
};

class DumpObject : public DumpObjectBase
{
protected:
    weak_ptr<WritableDump> dump;
    uint64_t savedOffset;
    uint32_t savedLength;

    DumpObject(weak_ptr<WritableDump> dump);
    virtual void WriteInternal() = 0;
    virtual void UpdateIndex(Offset offset, bool overwrite);
public:
    virtual void Write();
    virtual uint32_t NewLength() = 0;
    uint64_t SavedOffset() const;
};

template<typename T>
void DumpObjectBase::WriteValue(const T value)
{
    DumpTraits<T>::Write(*stream, value);
}

template<typename T>
void DumpObjectBase::ReadValue(std::istream &stream, T &value)
{
    value = DumpTraits<T>::Read(stream);
}

template<typename T>
uint32_t DumpObjectBase::ValueSize(const T value) const
{
    return DumpTraits<T>::DumpSize(value);
}