#include "CollectionHelpers.h"

void set(std::vector<bool> &vector, std::size_t index)
{
    if (vector.size() <= index)
        vector.resize(index + 1);

    vector.at(index) = true;
}

void unset(std::vector<bool> &vector, std::size_t index)
{
    if (vector.size() > index)
        vector.at(index) = false;
}

bool get(const std::vector<bool> &vector, std::size_t index)
{
    if (vector.size() <= index)
        return false;

    return vector.at(index);
}