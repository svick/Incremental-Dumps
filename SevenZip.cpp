#include <sstream>
#include "LZMA/CPP/7zip/Compress/LzmaEncoder.h"
#include "LZMA/CPP/7zip/Compress/LzmaDecoder.h"
#include "LZMA/CPP/7zip/Common/WrapperStreams.h"
#include "DumpException.h"
#include "SevenZip.h"

std::string properties("\x5D\x00\x00\x00\x01", 5);

std::string SevenZip::Compress(std::string s)
{
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

    HRESULT result = encoder.Code(&inputWrapperStream, &outputWrapperStream, nullptr, nullptr, nullptr);
    
    return outputStdStream.str();
}

std::string SevenZip::Decompress(std::string s)
{
    std::istringstream inputStdStream(s);
    CInWrapperStream inputWrapperStream(&inputStdStream);

    std::ostringstream outputStdStream;
    COutWrapperStream outputWrapperStream(&outputStdStream);

    NCompress::NLzma::CDecoder decoder;
    decoder.SetDecoderProperties2((const Byte*)properties.data(), properties.length());

    HRESULT result = decoder.Code(&inputWrapperStream, &outputWrapperStream, nullptr, nullptr, nullptr);

    // LZMA decompression seems to sometimes introduce trailing 0 byte
    // this takes care of that
    std::string decompressedString = outputStdStream.str();

    if (!decompressedString.empty() && decompressedString.back() == '\0')
        decompressedString.pop_back();

    return decompressedString;
}