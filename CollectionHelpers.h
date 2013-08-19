#pragma once

#include <vector>
#include <unordered_set>
#include <algorithm>

//http://stackoverflow.com/a/2330559/41071
template <typename T>
T& insert_at(T& pContainer, size_t pIndex, const typename T::value_type& pValue)
{
    pContainer.insert(pContainer.begin() + pIndex, pValue);

    return pContainer;
}

template <typename T>
T& insert_at(T& pContainer, size_t pIndex, typename T::value_type&& pValue)
{
    pContainer.insert(pContainer.begin() + pIndex, std::move(pValue));

    return pContainer;
}

template <typename T>
bool contains(T& container, const typename T::value_type& value)
{
    return std::find(container.begin(), container.end(), value) != container.end();
}

template <typename T>
std::unordered_set<T> except(std::vector<T> container1, std::vector<T> container2)
{
    std::unordered_set<T> set(container1.begin(), container1.end());

    for (auto item : container2)
        set.erase(item);

    return set;
}


void set(std::vector<bool> &vector, std::size_t index);

void unset(std::vector<bool> &vector, std::size_t index);

bool get(const std::vector<bool> &vector, std::size_t index);