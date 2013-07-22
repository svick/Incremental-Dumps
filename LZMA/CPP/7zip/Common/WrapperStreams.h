// WrapperStreams.h

#ifndef __WRAPPER_STREAMS_H
#define __WRAPPER_STREAMS_H

#include "../../Common/MyCom.h"

#include "../IStream.h"

#include <iostream>
#include <string>

class CInWrapperStream:
  public ISequentialInStream,
  public CMyUnknownImp
{
  std::istream *wrappedStream;
public:
  CInWrapperStream(std::istream *wrappedStream);

  MY_UNKNOWN_IMP

  virtual ~CInWrapperStream() {}
  STDMETHOD(Read)(void *data, UInt32 size, UInt32 *processedSize);
};

class COutWrapperStream:
  public ISequentialOutStream,
  public CMyUnknownImp
{
  std::ostream *wrappedStream;
public:
  COutWrapperStream(std::ostream *wrappedStream);

  MY_UNKNOWN_IMP

  virtual ~COutWrapperStream() {}
  STDMETHOD(Write)(const void *data, UInt32 size, UInt32 *processedSize);
};

#endif