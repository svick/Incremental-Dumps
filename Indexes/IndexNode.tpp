#include "IndexNode.h"
#include "IndexLeafNode.h"
#include "IndexInnerNode.h"
#include "../DumpException.h"
#include "../Dump.h"
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

    std::unique_ptr<IndexNode<TKey, TValue>> result;

    char byte;
    stream.read(&byte, 1);
    if (byte == (char)DumpObjectKind::IndexLeafNode)
    {
        result = IndexLeafNode<TKey, TValue>::Read(dump, stream);
    }
    else if (byte == (char)DumpObjectKind::IndexInnerNode)
    {
        result = IndexInnerNode<TKey, TValue>::Read(dump, stream);
    }
    else
        throw DumpException();

    result->savedOffset = offset;
    result->savedLength = result->NewLength();
    return result;
}

template<typename TKey, typename TValue>
unique_ptr<IndexNode<TKey, TValue>> IndexNode<TKey, TValue>::CreateNew(weak_ptr<WritableDump> dump)
{
    return unique_ptr<IndexNode<TKey, TValue>>(new IndexLeafNode<TKey, TValue>(dump));
}

template<typename TKey, typename TValue>
void IndexNode<TKey, TValue>::Write(Offset offset)
{
    auto dumpRef = dump.lock();

    if (savedOffset != 0)
        throw DumpException();

    stream = dumpRef->stream.get();
    stream->seekp(offset.value);

    WriteInternal();

    stream = nullptr;
}

template<typename TKey, typename TValue>
bool IndexNode<TKey, TValue>::IsOversized()
{
    return RealLength() > NewLength();
}
