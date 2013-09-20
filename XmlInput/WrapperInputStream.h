#include <iostream>
#include <functional>
#include "../XML/xmlfile.h"

class WrapperInputStream : public XML::InputStream
{
private:
    std::istream &wrapped;
    std::function<void(int)> sideAction;
public:
    WrapperInputStream(std::istream &wrapped, std::function<void()> sideAction)
        : wrapped(wrapped), sideAction([=](int){ sideAction(); })
    {}

    WrapperInputStream(std::istream &wrapped, std::function<void(int)> sideAction = nullptr)
        : wrapped(wrapped), sideAction(sideAction)
    {}

    virtual int read(XML_Char *buf, size_t bufLen) OVERRIDE
    {
        wrapped.read(buf, bufLen);
        auto count = wrapped.gcount();

        if (!wrapped.good())
        {
            // failure that's not end of stream
            if (!wrapped.eof() && wrapped.fail())
                throw DumpException("Reading from stream failed.");
        }

        if (sideAction != nullptr)
            sideAction(count);

        return count;
    }
};