#include "XmlUtils.h"

#include <sstream>
#include "DumpException.h"

// XMLIO doesn't seem to decode XML character entities, so this function does that
std::string unescape(std::string original)
{
    std::ostringstream stream;

    unsigned prevPos;
    unsigned pos = 0;

    while (pos < original.length())
    {
        prevPos = pos;

        int ampPos = original.find('&', pos);
        if (ampPos == std::string::npos)
        {
            stream << original.substr(pos);
            break;
        }

        pos = ampPos;
        stream << original.substr(prevPos, pos - prevPos);

        pos++;

        if (original.compare(pos, 3, "lt;") == 0)
        {
            stream << '<';
            pos += 3;
        }
        else if (original.compare(pos, 3, "gt;") == 0)
        {
            stream << '>';
            pos += 3;
        }
        else if (original.compare(pos, 4, "amp;") == 0)
        {
            stream << '&';
            pos += 4;
        }
        else if (original.compare(pos, 5, "quot;") == 0)
        {
            stream << '"';
            pos += 5;
        }
        else if (original.compare(pos, 5, "#039;") == 0 || original.compare(pos, 5, "apos;") == 0)
        {
            stream << '\'';
            pos += 5;
        }
    }

    return stream.str();
}

std::string readElementData(XML::Element &elem)
{
    if (elem.IsEmpty())
        return std::string();

    std::ostringstream stream;

    const size_t BUFFER_SIZE = 256;

    std::string buffer(BUFFER_SIZE, '\0');

    int read;

    do
    {
        read = elem.ReadData(&buffer.at(0), BUFFER_SIZE - 1);
        stream.write(buffer.data(), read);
    } while (read == BUFFER_SIZE);
    
    return unescape(stream.str());
}

std::string escapeElementText(std::string original)
{
    std::ostringstream stream;

    unsigned prevPos;
    unsigned pos = 0;

    while (pos < original.length())
    {
        prevPos = pos;

        int foundPos = original.find_first_of("<>\"&", pos);
        if (foundPos == std::string::npos)
        {
            stream << original.substr(pos);
            break;
        }

        pos = foundPos;
        stream << original.substr(prevPos, pos - prevPos);

        if (original.at(pos) == '<')
            stream << "&lt;";
        else if (original.at(pos) == '>')
            stream << "&gt;";
        else if (original.at(pos) == '"')
            stream << "&quot;";
        else if (original.at(pos) == '&')
            stream << "&amp;";
        else
            throw DumpException();

        pos++;
    }

    return stream.str();
}