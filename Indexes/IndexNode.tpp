#include "IndexNode.h"
#include "IndexLeafNode.h"
#include "../DumpException.h"
#include "../DumpObjects/DumpObjectKind.h"

template<typename TKey, typename TValue>
IndexNode<TKey, TValue>::IndexNode(weak_ptr<WritableDump> dump)
    : DumpObject(dump)
{}

template<typename TKey, typename TValue>
unique_ptr<IndexNode<TKey, TValue>> IndexNode<TKey, TValue>::Read(weak_ptr<WritableDump> dump, uint64_t offset)
{
    auto dumpRef = dump.lock();
    auto &stream = *(dumpRef->stream);
    stream.seekp(offset);

    char byte;
    stream.read(&byte, 1);
    if (byte == (char)DumpObjectKind::IndexLeafNode)
    {
        auto result = IndexLeafNode<TKey, TValue>::Read(dump, stream);
        result->savedOffset = offset;
        result->savedLength = result->NewLength();
        return result;
    }
    else
        throw new DumpException();
}

template<typename TKey, typename TValue>
unique_ptr<IndexNode<TKey, TValue>> IndexNode<TKey, TValue>::CreateNew(weak_ptr<WritableDump> dump)
{
    return unique_ptr<IndexNode<TKey, TValue>>(new IndexLeafNode<TKey, TValue>(dump));
}