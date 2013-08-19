#pragma once

#include <iostream>
#include "DumpObject.h"
#include "Offset.h"
#include "../DumpKind.h"

class ReadableDump;

class FileHeader : public DumpObject
{
private:
    static std::uint32_t Length();

    FileHeader(
        DumpKind kind,
        Offset fileEnd, Offset pageIdIndexRoot, Offset revisionIdIndexRoot, Offset modelFormatIndexRoot,
        Offset freeSpaceIndexRoot, Offset siteInfo, std::weak_ptr<WritableDump> dump = std::weak_ptr<WritableDump>());
protected:
    void WriteInternal();
public:
    static const std::uint8_t FileFormatVersion = 1;
    static const std::uint8_t FileDataVersion = 1;

    static FileHeader Read(ReadableDump const &dump);

    virtual void Write() override;
    virtual std::uint32_t NewLength() override;

    DumpKind Kind;

    Offset FileEnd;
    Offset PageIdIndexRoot;
    Offset RevisionIdIndexRoot;
    Offset ModelFormatIndexRoot;
    Offset FreeSpaceIndexRoot;
    Offset SiteInfo;

    FileHeader(std::weak_ptr<WritableDump> dump = std::weak_ptr<WritableDump>());
};