#include <sstream>
#include <cstdint>
#include "LZMA/CPP/7zip/Compress/LzmaEncoder.h"
#include "LZMA/CPP/7zip/Compress/LzmaDecoder.h"
#include "LZMA/CPP/7zip/Common/WrapperStreams.h"
#include "DumpException.h"
#include "SevenZip.h"

const std::string properties("\x5D\x00\x00\x00\x01", 5);

std::string SevenZip::Compress(const std::string &s)
{
    if (s.empty())
        return std::string();

    std::istringstream inputStdStream(s);
    CInWrapperStream inputWrapperStream(&inputStdStream);

    std::ostringstream outputStdStream;
    COutWrapperStream outputWrapperStream(&outputStdStream);

    NCompress::NLzma::CEncoder encoder;

#ifdef _DEBUG
    // verify that the default coder properties are as expected

    std::ostringstream propertiesStdStream;
    COutWrapperStream propertiesWrapperStream(&propertiesStdStream);
    encoder.WriteCoderProperties(&propertiesWrapperStream);

    if (propertiesStdStream.str() != properties)
        throw new DumpException();
#endif

    std::uint32_t result = encoder.Code(&inputWrapperStream, &outputWrapperStream, nullptr, nullptr, nullptr);

    if (result != SZ_OK)
        throw DumpException();
    
    return outputStdStream.str();
}

std::string SevenZip::Decompress(const std::string &s)
{
    if (s.empty())
        return std::string();

    std::istringstream inputStdStream(s);
    CInWrapperStream inputWrapperStream(&inputStdStream);

    std::ostringstream outputStdStream;
    COutWrapperStream outputWrapperStream(&outputStdStream);

    NCompress::NLzma::CDecoder decoder;
    decoder.SetDecoderProperties2((const Byte*)properties.data(), properties.length());

    std::uint32_t result = decoder.Code(&inputWrapperStream, &outputWrapperStream, nullptr, nullptr, nullptr);

    // SZ_ERROR_DATA is sometimes returned when decompression seemingly finished successfuly
    // I have no idea why or what to do about it
    if (result != SZ_OK && result != SZ_ERROR_DATA)
        throw DumpException();

    std::string decompressedString = outputStdStream.str();

    return decompressedString;
}