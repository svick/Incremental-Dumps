#pragma once

#include "Offset.h"
#include "IndexNode.h"

template<typename TKey, typename TValue>
class Index
{
private:
    unique_ptr<IndexNode<TKey, TValue>> rootNode;
    weak_ptr<WritableDump> dump;
    weak_ptr<Offset> fileHeaderOffset;
public:
    Index(weak_ptr<WritableDump> dump, weak_ptr<Offset> fileHeaderOffset);

    TValue operator[](TKey key);
    void Add(TKey key, TValue value);
};

#include "Index.hpp"