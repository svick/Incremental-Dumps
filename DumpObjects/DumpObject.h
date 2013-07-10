#pragma once

#include <cstdint>
#include <memory>
#include <iostream>
#include "Offset.h"

class WritableDump;

using std::uint64_t;
using std::unique_ptr;
using std::weak_ptr;
using std::ostream;

class DumpObject
{
private:
    ostream *stream;
protected:
    weak_ptr<WritableDump> dump;
    uint64_t savedOffset;
    uint32_t savedLength;

    DumpObject(weak_ptr<WritableDump> dump);
    virtual void WriteInternal() = 0;
    virtual void UpdateIndex(Offset offset);

    template<typename T>
    void WriteValue(const T value);

    template<typename T>
    uint32_t ValueSize(const T value) const;
public:
    virtual void Write();
    virtual uint32_t NewLength() const = 0;
    uint64_t SavedOffset() const;
};

template<typename T>
void DumpObject::WriteValue(const T value)
{
    DumpTraits<T>::Write(*stream, value);
}

template<typename T>
uint32_t DumpObject::ValueSize(const T value) const
{
    return DumpTraits<T>::DumpSize(value);
}