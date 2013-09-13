#include <iostream>
#include <functional>
#include "../XML/xmlfile.h"

class WrapperInputStream : public XML::InputStream
{
private:
    std::istream &wrapped;
    std::function<void()> sideAction;
public:
    WrapperInputStream(std::istream &wrapped, std::function<void()> sideAction = nullptr)
        : wrapped(wrapped), sideAction(sideAction)
    {}

    virtual int read(XML_Char *buf, size_t bufLen) OVERRIDE
    {
        if (sideAction != nullptr)
            sideAction();

        wrapped.read(buf, bufLen);
        return wrapped.gcount();
    }
};