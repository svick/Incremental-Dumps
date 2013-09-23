#pragma once

#include <cstdint>
#include <memory>
#include <iostream>
#include "Offset.h"
#include "DumpTraits.h"

class WritableDump;

/**
 * Abstract base class for all objects that can be written to disk.
 *
 * This includes normal objects that are written to the dump (DumpObject),
 * objects that are part of another object (DumpUser)
 * and objects that are not written to normal dump (Change).
 *
 * Apart from the listed members, derived classes commonly contain a static Read() method.
 */
class DumpObjectBase
{
protected:
    /**
     * The stream this object is being written to.
     *
     * This field is set only temporarily while writing the object to disk.
     */
    std::ostream *stream;

    /**
     * Writes this object to #stream.
     */
    virtual void WriteInternal() = 0;

    /**
     * Writes the given @a value to #stream using the appropriate DumpTraits specialization.
     */
    template<typename T>
    void WriteValue(const T &value);
public:
    /**
     * Writes the given @a value to the given @a stream using the appropriate DumpTraits specialization.
     */
    template<typename T>
    static void WriteValue(std::ostream &stream, const T &value);

    /**
     * Reads @a value from the given @a stream using the appropriate DumpTraits specialization.
     *
     * This method uses reference parameter instead of return value to make type inference work.
     */
    template<typename T>
    static void ReadValue(std::istream &stream, T &value);

    /**
     * Returns the size of the given @a value when written to disk, computed using the appropriate DumpTraits specialization.
     */
    template<typename T>
    static std::uint32_t ValueSize(const T &value);

    /**
     * The length of this object when written to disk.
     *
     * This takes into account any modifications made to this object,
     * it *doesn't* return the size this object had when it was read from disk.
     */
    virtual std::uint32_t NewLength() = 0;

    virtual ~DumpObjectBase() {}
};

/**
 * Abstract base class for objects that can be written to dump by themselves.
 */
class DumpObject : public DumpObjectBase
{
protected:
    /**
     * The dump this object belongs to.
     */
    std::weak_ptr<WritableDump> dump;
    /**
     * The offset this object is saved on from the start of the file.
     *
     * This is 0 for objects that weren't saved to the disk yet.
     */
    std::uint64_t savedOffset;
    /**
     * The length of this object on the disk.
     *
     * This does not reflect changes made since the object was read (unlike NewLength()).
     */
    std::uint32_t savedLength;

    DumpObject(std::weak_ptr<WritableDump> dump);
    virtual void WriteInternal() OVERRIDE = 0;
    /**
     * Updates the right index (or something like that) with information about this object.
     *
     * Usually, this is mapping from object ID to its offset in the file.
     * When this method is called, #savedOffset should be already updated to the new value
     * (if this object is being saved to a new location).
     *
     * @param overwrite If @c true, the current value in the index should be overwritten.
     *                  If @c false, there should be no value for this object in the index.
     */
    virtual void UpdateIndex(bool overwrite);
public:
    /**
     * Writes this obect to the dump.
     *
     * Takes care of allocating enough space in the file, sets #stream and then calls WriteInternal().
     */
    virtual void Write();
    virtual std::uint32_t NewLength() OVERRIDE = 0;
    /**
     * Returns #savedOffset.
     */
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