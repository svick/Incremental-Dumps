#include "XmlUtils.h"

#include <sstream>

string readElementData(XML::Element &elem)
{
    std::stringstream stream;

    const size_t BUFFER_SIZE = 10;

    char* buffer = new char[BUFFER_SIZE];

    int read;

    do
    {
        read = elem.ReadData(buffer, BUFFER_SIZE - 1);
        stream.write(buffer, read);
    } while (read == BUFFER_SIZE);
    
    return stream.str();
}