#pragma once

#include <map>
#include "IndexNode.h"

using std::map;

template<typename TKey, typename TValue>
class IndexLeafNode : public IndexNode<TKey, TValue>
{
private:
    static const int Size = 0xFFFF;

    map<TKey, TValue> map;
protected:
    virtual void WriteInternal();
public:
    static unique_ptr<IndexNode> Read(weak_ptr<WritableDump> dump, istream &stream);

    IndexLeafNode(weak_ptr<WritableDump> dump);

    using DumpObject::Write;
    virtual uint32_t NewLength() const;

    virtual TValue Get(TKey key);
    virtual void Add(TKey key, TValue value);
    virtual void AddOrUpdate(TKey key, TValue value);
    virtual void Remove(TKey key);

    virtual shared_ptr<IndexNodeIterator<TKey, TValue>> begin() const;
    virtual shared_ptr<IndexNodeIterator<TKey, TValue>> end() const;
};

#include "IndexLeafNode.tpp"