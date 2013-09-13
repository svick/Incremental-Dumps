// WrapperStreams.cpp

#include "StdAfx.h"

#include "WrapperStreams.h"

CInWrapperStream::CInWrapperStream(std::istream *wrappedStream)
    : wrappedStream(wrappedStream)
{}

STDMETHODIMP CInWrapperStream::Read(void *data, UInt32 size, UInt32 *processedSize)
{
    std::streamsize read = wrappedStream->readsome((char*)data, size);

    if (wrappedStream->fail())
        return E_FAIL;

    if (processedSize != NULL)
        *processedSize = read;

    return S_OK;
}

COutWrapperStream::COutWrapperStream(std::ostream *wrappedStream)
    : wrappedStream(wrappedStream)
{}

STDMETHODIMP COutWrapperStream::Write(const void *data, UInt32 size, UInt32 *processedSize)
{
    if (processedSize != NULL)
        *processedSize = 0;

    wrappedStream->write((char*)data, size);

    if (wrappedStream->fail())
        return E_FAIL;

    if (processedSize != NULL)
        *processedSize = size;

    return S_OK;
}
