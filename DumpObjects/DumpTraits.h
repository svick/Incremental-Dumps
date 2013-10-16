#pragma once

#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <set>
#include <map>
#include "../DumpException.h"

/**
 * Template used for reading and writing of various data types to the dump.
 *
 * This template has specializations for all types that can be read and written to the dump.
 * The second template parameter is for conditions (e.g. using @c std::enable_if).
 *
 * @tparam T The type that is read and written by this template.
 */
template<typename T, typename = void>
class DumpTraits
{
public:
    /**
     * Reads the given type from the stream and returns it.
     */
    static T Read(std::istream &stream)
    {
        return T::Read(stream);
    }

    /**
     * Writes the given value to the stream.
     */
    static void Write(std::ostream &stream, const T& value)
    {
        value.Write(stream);
    }

    /**
     * Returns the size the given value will have when written.
     */
    static uint32_t DumpSize(const T& value)
    {
        return DumpSize();
    }
};

/**
 * Helper template for reading and writing of numeric types to the dump.
 *
 * This is not simply a specialization of DumpTraits, because of the @a Size template parameter.
 * This parameter is used for example in DumpTraits<Offset>.
 *
 * A specialization fo DumpTraits is created by inheriting from this template.
 *
 * @tparam Size How many bytes will this type occupy on disk.
 */
template<
    typename T,
    std::uint32_t Size = sizeof(T),
    typename = typename std::enable_if<std::is_integral<T>::value>::type>
class DumpTraitsNumeric
{
public:
    static T Read(std::istream &stream)
    {
        std::array<char, Size> bytes;

        stream.read(bytes.data(), Size);

        T result = 0;
        for (int i = 0; i < Size; i++)
        {
            result |= (T)(std::uint8_t)bytes.at(i) << (8 * i);
        }

        return result;
    }

    static void Write(std::ostream &stream, const T value)
    {
        std::array<char, Size> bytes;

        for (int i = 0; i < Size; i++)
        {
            bytes.at(i) = (value >> (8 * i)) & 0xFF;
        }

        stream.write(bytes.data(), Size);
    }

    static std::uint32_t DumpSize(const T value = 0)
    {
        return Size;
    }
};

template<typename T>
class DumpTraits<T, typename std::enable_if<std::is_integral<T>::value>::type>
    : public DumpTraitsNumeric<T>
{};

/**
 * Specializations of this template are used for converting from one type another before writing to the dump
 * (and vice versa after reading).
 *
 * Each specialization has three members:
 * - Member type @c Target, which specifies to what type is this type converted before saving
 * - Function <tt>static Target Convert(const Source)</tt>
 * - Function <tt>static Source ConvertBack(const Target)</tt>
 *
 * @tparam Source The type that is converted by this template.
 */
template<typename Source>
class DumpConverter
{};

/**
 * Helper template for reading and writing of types that have to be converted.
 *
 * A specialization fo DumpTraits is created by inheriting from this template.
 *
 * @tparam TargetTraits The DumpTraits type that will be used to read and write the converted value.
 */
template<
    typename T,
    typename TargetTraits = DumpTraits<typename DumpConverter<T>::Target>>
class DumpTraitsConverted
{
public:
    static T Read(std::istream &stream)
    {
        auto target = TargetTraits::Read(stream);
        return DumpConverter<T>::ConvertBack(target);
    }

    static void Write(std::ostream &stream, const T& value)
    {
        auto target = DumpConverter<T>::Convert(value);
        TargetTraits::Write(stream, target);
    }

    static std::uint32_t DumpSize(const T& value = T())
    {
        auto target = DumpConverter<T>::Convert(value);
        return TargetTraits::DumpSize(target);
    }
};

template<>
class DumpConverter<std::int16_t>
{
public:
    typedef std::uint16_t Target;

    static std::uint16_t Convert(const std::int16_t source)
    {
        return source;
    }

    static std::int16_t ConvertBack(const std::uint16_t target)
    {
        return target;
    }
};

template<>
class DumpTraits<std::int16_t> : public DumpTraitsConverted<std::int16_t>
{};

template<>
class DumpConverter<Offset>
{
public:
    typedef std::uint64_t Target;

    static std::uint64_t Convert(const Offset source)
    {
        return source.value;
    }

    static Offset ConvertBack(const std::uint64_t target)
    {
        return target;
    }
};

template<>
class DumpTraits<Offset> : public DumpTraitsConverted<Offset, DumpTraitsNumeric<std::uint64_t, 6>>
{};

template<typename T>
class DumpTraits<T, typename std::enable_if<std::is_enum<T>::value>::type>
{
#ifdef GCC_4_6
    typedef std::uint64_t UnderlyingType;
    typedef DumpTraitsNumeric<UnderlyingType, sizeof(T)> TargetTraits;
#else
    typedef typename std::underlying_type<T>::type UnderlyingType;
    typedef DumpTraits<UnderlyingType> TargetTraits;
#endif

public:
    static T Read(std::istream &stream)
    {
        auto target = TargetTraits::Read(stream);
        return static_cast<T>(target);
    }

    static void Write(std::ostream &stream, const T value)
    {
        auto target = static_cast<UnderlyingType>(value);
        TargetTraits::Write(stream, target);
    }

    static uint32_t DumpSize(const T value = T())
    {
        auto target = static_cast<UnderlyingType>(value);
        return TargetTraits::DumpSize(target);
    }
};

/**
 * The methods for this specialization come in two variants: normal (short) and long.
 *
 * The short variants are for strings of up to 255 bytes.
 * The long variants are for strings of up to 4 GB.
 */
template<>
class DumpTraits<std::string>
{
private:
    static std::string ReadCore(std::istream &stream, std::uint32_t count)
    {
        std::string bytes(count, '\0');

        if (count > 0)
            stream.read(&bytes.at(0), count);

        return bytes;
    }

public:
    static std::string Read(std::istream &stream)
    {
        uint8_t count = DumpTraits<std::uint8_t>::Read(stream);

        return ReadCore(stream, count);
    }

    static std::string ReadLong(std::istream &stream)
    {
        uint32_t count = DumpTraits<std::uint32_t>::Read(stream);

        return ReadCore(stream, count);
    }

    static void Write(std::ostream &stream, const std::string& value)
    {
        auto length = value.length();

        if (length > 255)
        {
            throw DumpException();
        }
        else
        {
            DumpTraits<std::uint8_t>::Write(stream, length);

            stream.write(value.data(), length);
        }
    }

    static void WriteLong(std::ostream &stream, const std::string& value)
    {
        auto length = value.length();

        if (length > 0xFFFFFFFF)
        {
            throw DumpException();
        }

        DumpTraits<uint32_t>::Write(stream, length);

        stream.write(value.data(), length);
    }

    static uint32_t DumpSize(const std::string& value)
    {
        return DumpTraits<std::uint8_t>::DumpSize(value.length()) + value.length();
    }

    static uint32_t DumpSizeLong(const std::string& value)
    {
        return DumpTraits<std::uint32_t>::DumpSize(value.length()) + value.length();
    }
};

template<typename T>
class DumpTraits<std::vector<T>>
{
public:
    static std::vector<T> Read(std::istream &stream)
    {
        std::uint32_t count = DumpTraits<std::uint32_t>::Read(stream);

        std::vector<T> result;

        for (std::uint32_t i = 0; i < count; i++)
        {
            result.push_back(DumpTraits<T>::Read(stream));
        }

        return result;
    }

    static void Write(std::ostream &stream, const std::vector<T> &value)
    {
        auto length = value.size();

        if (length >= std::numeric_limits<std::uint32_t>::max())
            throw DumpException();

        DumpTraits<std::uint32_t>::Write(stream, length);

        for (T item : value)
        {
            DumpTraits<T>::Write(stream, item);
        }
    }

    static std::uint32_t DumpSize(const std::vector<T> &value)
    {
        std::uint32_t size = DumpTraits<std::uint32_t>::DumpSize(value.size());

        for (T item : value)
        {
            size += DumpTraits<T>::DumpSize(item);
        }

        return size;
    }
};

template<typename T, size_t N>
class DumpTraits<std::array<T, N>>
{
public:
    static std::array<T, N> Read(std::istream &stream)
    {
        std::array<T, N> result;

        for (int i = 0; i < N; i++)
        {
            result.at(i) = DumpTraits<T>::Read(stream);
        }

        return result;
    }

    static void Write(std::ostream &stream, const std::array<T, N> &value)
    {
        for (T item : value)
        {
            DumpTraits<T>::Write(stream, item);
        }
    }

    static std::uint32_t DumpSize(const std::array<T, N> &value)
    {
        std::uint32_t size = 0;

        for (T item : value)
        {
            size += DumpTraits<T>::DumpSize(item);
        }

        return size;
    }
};

template<typename T>
class DumpTraits<std::set<T>>
{
public:
    static std::set<T> Read(std::istream &stream)
    {
        std::uint32_t count = DumpTraits<std::uint32_t>::Read(stream);

        std::set<T> result;

        for (std::uint32_t i = 0; i < count; i++)
        {
            result.insert(result.end(), DumpTraits<T>::Read(stream));
        }

        return result;
    }

    static void Write(std::ostream &stream, const std::set<T> &value)
    {
        auto length = value.size();

        if (length >= std::numeric_limits<std::uint32_t>::max())
            throw DumpException();

        DumpTraits<std::uint32_t>::Write(stream, length);

        for (T item : value)
        {
            DumpTraits<T>::Write(stream, item);
        }
    }

    static std::uint32_t DumpSize(const std::set<T> &value)
    {
        std::uint32_t size = DumpTraits<std::uint32_t>::DumpSize(value.size());

        for (T item : value)
        {
            size += DumpTraits<T>::DumpSize(item);
        }

        return size;
    }
};

template<typename TKey, typename TValue>
class DumpTraits<std::map<TKey, TValue>>
{
public:
    static std::map<TKey, TValue> Read(std::istream &stream)
    {
        std::uint16_t count = DumpTraits<std::uint16_t>::Read(stream);

        std::map<TKey, TValue> result;

        for (int i = 0; i < count; i++)
        {
            auto key = DumpTraits<TKey>::Read(stream);
            auto value = DumpTraits<TValue>::Read(stream);
            result.insert(std::make_pair(key, value));
        }

        return result;
    }

    static void Write(std::ostream &stream, const std::map<TKey, TValue> &value)
    {
        auto length = value.size();

        if (length >= std::numeric_limits<std::uint16_t>::max())
            throw DumpException();

        DumpTraits<std::uint16_t>::Write(stream, length);

        for (auto pair : value)
        {
            DumpTraits<TKey>::Write(stream, pair.first);
            DumpTraits<TValue>::Write(stream, pair.second);
        }
    }

    static std::uint32_t DumpSize(const std::map<TKey, TValue> &value)
    {
        std::uint32_t size = DumpTraits<std::uint16_t>::DumpSize(value.size());

        for (auto pair : value)
        {
            size += DumpTraits<TKey>::DumpSize(pair.first) + DumpTraits<TValue>::DumpSize(pair.second);
        }

        return size;
    }
};

template<typename T1, typename T2>
class DumpTraits<std::pair<T1, T2>>
{
public:
    static std::pair<T1, T2> Read(std::istream &stream)
    {
        T1 first = DumpTraits<T1>::Read(stream);
        T2 second = DumpTraits<T2>::Read(stream);

        return std::make_pair(first, second);
    }

    static void Write(std::ostream &stream, const std::pair<T1, T2> &value)
    {
        DumpTraits<T1>::Write(stream, value.first);
        DumpTraits<T2>::Write(stream, value.second);
    }

    static std::uint32_t DumpSize(const std::pair<T1, T2> &value)
    {
        return DumpTraits<T1>::DumpSize(value.first) + DumpTraits<T2>::DumpSize(value.second);
    }
};