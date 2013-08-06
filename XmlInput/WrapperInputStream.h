#include <iostream>
#include "../XML/xmlfile.h"

class WrapperInputStream : public XML::InputStream
{
private:
    std::istream &wrapped;
public:
    WrapperInputStream(std::istream &wrapped)
        : wrapped(wrapped)
    {}

    virtual int read(XML_Char *buf, size_t bufLen) override
    {
        wrapped.read(buf, bufLen);
        return wrapped.gcount();
    }
};