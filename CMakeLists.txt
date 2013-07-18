cmake_minimum_required(VERSION 2.8)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-std=c++11)
endif()

# File list: C/C++ Compiler
set(SOURCES_files_ClCompile
  Dump.cpp
  DumpException.cpp
  DumpObjects/DumpIpV4User.cpp
  DumpObjects/DumpNamedUser.cpp
  DumpObjects/DumpObject.cpp
  DumpObjects/DumpTraits.cpp
  DumpObjects/FileHeader.cpp
  DumpObjects/DumpPage.cpp
  DumpObjects/DumpRevision.cpp
  DumpObjects/DumpUser.cpp
  Objects/IpV4User.cpp
  main.cpp
  DumpObjects/Offset.cpp
  Objects/Page.cpp
  Objects/Revision.cpp
  SpaceManager.cpp
  DumpWriters/TestDumpWriter.cpp
  DumpWriters/StubCurrentWriter.cpp
  Objects/User.cpp
  StringHelpers.cpp
  Objects/Timestamp.cpp
  XmlContributorProcessor.cpp
  XmlPageProcessor.cpp
  XmlRevisionProcessor.cpp
  XmlUtils.cpp
  XML/xmlfile.cpp
  XML/xmlfile_c.c
  XML/xmlinput.cpp
  XML/xmlinput_c.c
  XML/xmloutput.cpp
)
# File list: C/C++ Header
set(SOURCES_files_ClInclude
  DumpException.h
  DumpObjects/DumpIpV4User.h
  DumpObjects/DumpNamedUser.h
  DumpObjects/DumpObjectKind.h
  DumpObjects/DumpPage.h
  DumpObjects/DumpRevision.h
  DumpObjects/DumpUser.h
  Indexes/Index.h
  Indexes/IndexLeafNode.tpp
  Indexes/IndexNode.tpp
  Indexes/Index.tpp
  Dump.h
  DumpObjects/DumpObject.h
  DumpObjects/DumpTraits.h
  DumpWriters/DumpWriter.h
  DumpObjects/FileHeader.h
  Indexes/IndexLeafNode.h
  Indexes/IndexNode.h
  DumpObjects/Offset.h
  Objects/IpV4User.h
  Objects/Page.h
  Objects/Revision.h
  Indexes/Iterators/IndexIterator.h
  Indexes/Iterators/IndexIterator.tpp
  Indexes/Iterators/IndexLeafIterator.h
  Indexes/Iterators/IndexLeafIterator.tpp
  Indexes/Iterators/IndexNodeIterator.h
  Indexes/Iterators/IndexNodeIterator.tpp
  SpaceManager.h
  DumpWriters/TestDumpWriter.h
  DumpWriters/StubCurrentWriter.h
  Objects/User.h
  StringHelpers.h
  Objects/Timestamp.h
  XmlContributorProcessor.h
  XmlPageProcessor.h
  XmlRevisionProcessor.h
  XmlUtils.h
  XML/xmlconfig.h
  XML/xmlfile.h
  XML/xmlinput.h
  XML/xmlinputp.h
  XML/xmloutput.h
  XML/xmlstream.h
)
set_property(SOURCE ${SOURCES_files_ClInclude} PROPERTY HEADER_FILE_ONLY
  ON
)
set(fl_want_add_to_target ON)

set(SOURCES
  ${SOURCES_files_ClCompile}
  ${SOURCES_files_ClInclude}
)

if(NOT TARGET Incrementaldumps)
  add_executable(Incrementaldumps ${SOURCES})
endif(NOT TARGET Incrementaldumps)

if(TARGET Incrementaldumps)
  set(CMAKE_VS_PLATFORM_TOOLSET
    v110
  )
endif(TARGET Incrementaldumps)
