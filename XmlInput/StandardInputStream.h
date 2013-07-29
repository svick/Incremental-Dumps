#include <iostream>
#include "../XML/xmlfile.h"

class StandardInputStream : public XML::InputStream
{
public:
    virtual int read(XML_Char *buf, size_t bufLen) override
    {
        std::cin.read(buf, bufLen);
        return std::cin.gcount();
    }
};