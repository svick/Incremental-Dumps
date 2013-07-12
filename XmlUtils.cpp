#include "XmlUtils.h"

#include <sstream>

using std::ostringstream;

// XMLIO doesn't seem to decode XML character entities, so this function does that
string unescape(string original)
{
    ostringstream stream;

    unsigned prevPos;
    unsigned pos = 0;

    while (pos < original.length())
    {
        prevPos = pos;

        int ampPos = original.find('&', pos);
        if (ampPos == string::npos)
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

string readElementData(XML::Element &elem)
{
    ostringstream stream;

    const size_t BUFFER_SIZE = 256;

    char* buffer = new char[BUFFER_SIZE];

    int read;

    do
    {
        read = elem.ReadData(buffer, BUFFER_SIZE - 1);
        stream.write(buffer, read);
    } while (read == BUFFER_SIZE);
    
    return unescape(stream.str());
}