#pragma once

#include <map>
#include "IndexNode.h"

using std::map;

template<typename TKey, typename TValue>
class IndexLeafNode : public IndexNode<TKey, TValue>
{
private:
    map<TKey, TValue> map;
protected:
    virtual void Write(ostream &stream);
public:
    static unique_ptr<IndexNode> Read(weak_ptr<WritableDump> dump, istream &stream);

    IndexLeafNode(weak_ptr<WritableDump> dump);

    using DumpObject::Write;
    virtual int64_t NewLength();

    virtual TValue operator[](TKey key);
    virtual void Add(TKey key, TValue value);
};

#include "IndexLeafNode.hpp"