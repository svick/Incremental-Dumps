#include "FetchText.h"
#include "DumpException.h"

std::string FetchText::GetText(std::uint32_t textId)
{
    auto &in = stream->in();
    in << textId << std::endl;
    in.flush();

    auto &out = stream->out();
    std::uint32_t readId;
    out >> readId;

    if (readId != textId)
        throw DumpException();

    std::uint32_t length;
    out >> length;

    // ignore \n remaining from >>
    out.ignore();

    std::string result(length, '\0');
    out.read(&result[0], length);

    return result;
}