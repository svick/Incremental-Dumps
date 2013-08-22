cmake_minimum_required(VERSION 2.8)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-std=c++11)
endif()

# File list: C/C++ Compiler
set(SOURCES_files_ClCompile
  Diff/ChangeProcessor.cpp
  Diff/Changes/Change.cpp
  Diff/Changes/DeletePageChange.cpp
  Diff/Changes/DeleteRevisionChange.cpp
  Diff/Changes/NewModelFormatChange.cpp
  Diff/Changes/NewPageChange.cpp
  Diff/Changes/NewRevisionChange.cpp
  Diff/Changes/PageChange.cpp
  Diff/Changes/RevisionChange.cpp
  Diff/Changes/SiteInfoChange.cpp
  Diff/DiffReader.cpp
  Diff/DiffWriter.cpp
  CollectionHelpers.cpp
  Dump.cpp
  DumpException.cpp
  DumpKind.cpp
  DumpObjects/DumpIpV4User.cpp
  DumpObjects/DumpIpV6User.cpp
  DumpObjects/DumpNamedUser.cpp
  DumpObjects/DumpObject.cpp
  DumpObjects/DumpSiteInfo.cpp
  DumpObjects/FileHeader.cpp
  DumpObjects/DumpPage.cpp
  DumpObjects/DumpRevision.cpp
  DumpObjects/DumpUser.cpp
  Objects/IpV4User.cpp
  Objects/IpV6User.cpp
  Objects/NamedUser.cpp
  main.cpp
  DumpObjects/Offset.cpp
  Objects/Page.cpp
  Objects/Revision.cpp
  SevenZip.cpp
  SpaceManager.cpp
  DumpWriters/ArticlesWriterWrapper.cpp
  DumpWriters/CompositeWriter.cpp
  DumpWriters/CurrentWriterWrapper.cpp
  DumpWriters/DumpWriter.cpp
  DumpWriters/WriterWrapper.cpp
  FetchText.cpp
  format.cc
  libexecstream/exec-stream.cpp
  Objects/User.cpp
  StringHelpers.cpp
  Objects/Timestamp.cpp
  XmlInput/XmlContributorProcessor.cpp
  XmlInput/XmlMediawikiProcessor.cpp
  XmlInput/XmlNamespacesProcessor.cpp
  XmlInput/XmlPageProcessor.cpp
  XmlInput/XmlRevisionProcessor.cpp
  XmlInput/XmlSiteInfoProcessor.cpp
  XmlUtils.cpp
  XmlWriter.cpp
  XML/xmlfile.cpp
  XML/xmlfile_c.c
  XML/xmlinput.cpp
  XML/xmlinput_c.c
  XML/xmloutput.cpp
)
# File list: C/C++ Header
set(SOURCES_files_ClInclude
  Diff/ChangeProcessor.h
  Diff/Changes/Change.h
  Diff/Changes/DeletePageChange.h
  Diff/Changes/DeleteRevisionChange.h
  Diff/Changes/NewModelFormatChange.h
  Diff/Changes/NewPageChange.h
  Diff/Changes/NewRevisionChange.h
  Diff/Changes/PageChange.h
  Diff/Changes/RevisionChange.h
  Diff/Changes/SiteInfoChange.h
  Diff/DiffReader.h
  Diff/DiffWriter.h
  CollectionHelpers.h
  DumpException.h
  DumpKind.h
  DumpObjects/DumpIpV4User.h
  DumpObjects/DumpIpV6User.h
  DumpObjects/DumpNamedUser.h
  DumpObjects/DumpObjectKind.h
  DumpObjects/DumpPage.h
  DumpObjects/DumpRevision.h
  DumpObjects/DumpSiteInfo.h
  DumpObjects/DumpUser.h
  Indexes/Index.h
  Indexes/IndexInnerNode.tpp
  Indexes/IndexLeafNode.tpp
  Indexes/IndexNode.tpp
  Indexes/Index.tpp
  Dump.h
  DumpObjects/DumpObject.h
  DumpObjects/DumpTraits.h
  DumpObjects/FileHeader.h
  Indexes/IndexInnerNode.h
  Indexes/IndexLeafNode.h
  Indexes/IndexNode.h
  DumpObjects/Offset.h
  Objects/IpV4User.h
  Objects/IpV6User.h
  Objects/NamedUser.h
  Objects/Page.h
  Objects/Revision.h
  Objects/SiteInfo.h
  Indexes/Iterators/IndexIterator.h
  Indexes/Iterators/IndexIterator.tpp
  Indexes/Iterators/IndexInnerIterator.h
  Indexes/Iterators/IndexInnerIterator.tpp
  Indexes/Iterators/IndexLeafIterator.h
  Indexes/Iterators/IndexLeafIterator.tpp
  Indexes/Iterators/IndexNodeIterator.h
  libexecstream/exec-stream.h
  SevenZip.h
  SpaceManager.h
  DumpWriters/ArticlesWriterWrapper.h
  DumpWriters/CompositeWriter.h
  DumpWriters/CurrentWriterWrapper.h
  DumpWriters/DumpWriter.h
  DumpWriters/IDumpWriter.h
  DumpWriters/WriterWrapper.h
  FetchText.h
  format.h
  Objects/User.h
  StringHelpers.h
  Objects/Timestamp.h
  XmlInput/WrapperInputStream.h
  XmlInput/XmlContributorProcessor.h
  XmlInput/XmlMediawikiProcessor.h
  XmlInput/XmlNamespacesProcessor.h
  XmlInput/XmlPageProcessor.h
  XmlInput/XmlRevisionProcessor.h
  XmlInput/XmlSiteInfoProcessor.h
  XmlUtils.h
  XmlWriter.h
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

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
cmake_policy(SET CMP0015 OLD)
link_directories(LZMA/CPP/7zip/Bundles/LzmaCon)

add_executable(idumps ${SOURCES})

target_link_libraries(idumps "lzma.a")
target_link_libraries(idumps "pthread.so")

if(CMAKE_COMPILER_IS_GNUCXX)
  add_custom_command(TARGET idumps
    PRE_LINK
    COMMAND make -f makefile.gcc
    WORKING_DIRECTORY LZMA/CPP/7zip/Bundles/LzmaCon)
endif()

set(CMAKE_VS_PLATFORM_TOOLSET v110)
