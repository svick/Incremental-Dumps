#pragma once

#include <string>

class SevenZip
{
private:
    SevenZip() {}
public:
    static std::string Compress(std::string s);
    static std::string Decompress(std::string s);
};