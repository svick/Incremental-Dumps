#include "IndexNode.h"
#include "IndexLeafNode.h"
#include "IndexInnerNode.h"
#include "../DumpException.h"
#include "../Dump.h"
#include "../DumpObjects/DumpObjectKind.h"

template<typename TKey, typename TValue>
IndexNode<TKey, TValue>::IndexNode(std::weak_ptr<Dump> dump)
    : DumpObject(dump), modified(false), iterators(0)
{}

template<typename TKey, typename TValue>
std::unique_ptr<IndexNode<TKey, TValue>> IndexNode<TKey, TValue>::Read(std::weak_ptr<Dump> dump, std::uint64_t offset)
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
std::unique_ptr<IndexNode<TKey, TValue>> IndexNode<TKey, TValue>::CreateNew(std::weak_ptr<Dump> dump)
{
    return std::unique_ptr<IndexNode<TKey, TValue>>(new IndexLeafNode<TKey, TValue>(dump));
}

template<typename TKey, typename TValue>
void IndexNode<TKey, TValue>::Write()
{
    Write(false);
}

template<typename TKey, typename TValue>
void IndexNode<TKey, TValue>::Write(bool force)
{
    if (!modified && !force)
        return;

    DumpObject::Write();

    modified = false;
}

template<typename TKey, typename TValue>
bool IndexNode<TKey, TValue>::IsOversized()
{
    return RealLength() > NewLength();
}

template<typename TKey, typename TValue>
bool IndexNode<TKey, TValue>::CanBeCleared()
{
    return iterators == 0;
}

template<typename TKey, typename TValue>
void IndexNode<TKey, TValue>::ClearCached()
{
    Write();

    ClearCachedInternal();
}