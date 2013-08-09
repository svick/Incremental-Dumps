#pragma once

#include "DumpObject.h"
#include "../Dump.h"
#include "../Objects/Revision.h"

// after DumpRevision is created, Text of its revision can't be changed
class DumpRevision : public DumpObject
{
private:
    bool withText;
    std::string compressedText;

    RevisionFlags originalFlags;
    std::uint32_t originalParentId;
    bool wasLoaded;
    std::uint8_t modelFormatId;

    std::uint32_t textLength;
    std::uint32_t textOffset;
    bool textUnloaded;

    void Load(std::uint32_t revisionId, bool loadText);
    Revision Read(std::shared_ptr<WritableDump> dump, Offset offset, bool loadText);
    void EnsureCompressed();
protected:
    virtual void WriteInternal() override;
    virtual void UpdateIndex(Offset offset, bool overwrite) override;
public:
    Revision revision;

    DumpRevision(std::weak_ptr<WritableDump> dump, std::uint32_t revisionId, bool loadText);
    DumpRevision(std::weak_ptr<WritableDump> dump);

    virtual void Write() override;
    virtual std::uint32_t NewLength() override;
};