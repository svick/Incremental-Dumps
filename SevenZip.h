#pragma once

#include <string>

class SevenZip
{
private:
    SevenZip() {}
public:
    static std::string Compress(const std::string &s);
    static std::string Decompress(const std::string &s);
};