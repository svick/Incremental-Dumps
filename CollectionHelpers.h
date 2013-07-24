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