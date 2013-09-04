#pragma once

#include "DumpObject.h"
#include "../Dump.h"
#include "../Objects/Revision.h"
#include "../Diff/DiffWriter.h"

class DumpRevision : public DumpObject
{
private:
    bool withText;

    Revision originalRevision;
    bool wasLoaded;
    std::uint8_t modelFormatId;
    bool isModelFormatIdNew;

    std::uint32_t textGroupId;
    std::uint8_t textId;

    std::uint32_t textLength;
    std::uint64_t textOffset;
    bool textUnloaded;

    DiffWriter *diffWriter;
    bool forceDiff;

    void Load(std::uint32_t revisionId);
    Revision Read(std::shared_ptr<WritableDump> dump, Offset offset);
protected:
    virtual void WriteInternal() override;
    virtual void UpdateIndex(Offset offset, bool overwrite) override;
public:
    Revision revision;

    DumpRevision(std::weak_ptr<WritableDump> dump, std::uint32_t revisionId);

    std::uint8_t GetModelFormatId(bool &isNew);
    void SetModelFormatId(std::uint8_t modelFormatId);

    virtual void Write() override;
    // forceDiff set to true forces writing into diff, even if the revision didn't change
    // this is necessary when revision moves between pages because of undeletion
    void Write(DiffWriter *diffWriter, bool forceDiff);
    virtual std::uint32_t NewLength() override;

    static Revision ReadCore(std::istream &stream, std::uint8_t &modelFormatId, bool withText);
    static void WriteCore(std::ostream &stream, Revision &revision, std::uint8_t modelFormatId, bool withText);
    static std::uint32_t LengthCore(const Revision &revision, std::uint8_t modelFormatId, bool withText);

    // deletes text of the current revision from the dump, if there is any
    void DeleteText();
};