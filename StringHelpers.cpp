#include "StringHelpers.h"
#include <sstream>

using std::stringstream;

// http://stackoverflow.com/a/236803/41071
vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}