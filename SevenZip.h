#pragma once

#include <string>

/**
 * Used to compress and decompress strings using the LZMA algorithm.
 */
class SevenZip
{
private:
    SevenZip() {}
public:
    /**
     * Compresses the given string using LZMA.
     */
    static std::string Compress(const std::string &s);
    /**
     * Decompresses the given string using LZMA.
     */
    static std::string Decompress(const std::string &s);
};