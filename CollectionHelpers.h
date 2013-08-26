#pragma once

#include <vector>
#include <set>
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
bool contains(const std::set<T> &container, const T &value)
{
    return container.count(value) > 0;
}

template <typename T>
std::unordered_set<T> except(const std::vector<T> &container1, const std::vector<T> &container2)
{
    std::unordered_set<T> set(container1.begin(), container1.end());

    for (auto item : container2)
        set.erase(item);

    return set;
}

template <typename T>
std::set<T> except(const std::set<T> &container1, const std::set<T> &container2)
{
    std::set<T> result = container1;

    for (auto item : container2)
        result.erase(item);

    return result;
}

void set(std::vector<bool> &vector, std::size_t index);

void unset(std::vector<bool> &vector, std::size_t index);

bool get(const std::vector<bool> &vector, std::size_t index);