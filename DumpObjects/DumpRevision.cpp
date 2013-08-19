#include "DumpRevision.h"
#include "DumpObjectKind.h"
#include "DumpUser.h"
#include "../SevenZip.h"
#include "../Indexes/Index.h"

void DumpRevision::Load(std::uint32_t revisionId, bool loadText)
{
    auto dumpRef = dump.lock();
    auto revisionOffset = dumpRef->revisionIdIndex->Get(revisionId);
    if (revisionOffset.value == 0)
    {
        revision = Revision();
        savedOffset = 0;
        savedLength = 0;
        wasLoaded = false;
    }
    else
    {
        revision = Read(dumpRef, revisionOffset, loadText);

        originalRevision = revision;

        savedOffset = revisionOffset.value;
        savedLength = NewLength();
    }
}

Revision DumpRevision::Read(std::shared_ptr<WritableDump> dump, Offset offset, bool loadText)
{
    auto &stream = *(dump->stream);
    stream.seekp(offset.value);

    auto kind = DumpTraits<uint8_t>::Read(stream);
    if (kind != (uint8_t)DumpObjectKind::Revision)
        throw new DumpException();

    std::uint8_t modelFormatId;
    auto revision = ReadCore(stream, modelFormatId, withText, loadText);

    if (HasFlag(revision.Flags, RevisionFlags::WikitextModelFormat))
    {
        revision.Model = WritableDump::WikitextModel;
        revision.Format = WritableDump::WikitextFormat;
    }
    else
    {
        auto modelFormat = dump->GetModelFormat(modelFormatId);
        revision.Model = modelFormat.first;
        revision.Format = modelFormat.second;
    }

    if (!HasFlag(revision.Flags, RevisionFlags::TextDeleted) && withText && !loadText)
    {
        textOffset = stream.tellg();
        ReadValue(stream, textLength);
        textUnloaded = true;
    }

    return revision;
}

void DumpRevision::Write()
{
    bool ignored;
    GetModelFormatId(ignored);

    if (wasLoaded && originalRevision == revision)
    {
        if (forceDiff && diffWriter != nullptr)
            diffWriter->ChangeRevision(originalRevision, revision, modelFormatId);

        return;
    }

    if (textUnloaded)
    {
        auto dumpRef = dump.lock();
        auto stream = dumpRef->stream.get();
        stream->seekp(textOffset);
        revision.SetCompressedText(DumpTraits<string>::ReadLong(*stream));
    }

    DumpObject::Write();
}

void DumpRevision::Write(DiffWriter *diffWriter, bool forceDiff)
{
    this->diffWriter = diffWriter;
    this->forceDiff = forceDiff;
    Write();
    this->diffWriter = nullptr;
    this->forceDiff = false;
}

void DumpRevision::WriteInternal()
{
    WriteValue(DumpObjectKind::Revision);
    WriteCore(*stream, revision, modelFormatId, withText);

    if (diffWriter != nullptr)
    {
        if (wasLoaded)
            diffWriter->ChangeRevision(originalRevision, revision, modelFormatId);
        else
            diffWriter->NewRevision(revision, modelFormatId);
    }
}

void DumpRevision::UpdateIndex(Offset offset, bool overwrite)
{
    auto dumpRef = dump.lock();

    if (overwrite)
        dumpRef->revisionIdIndex->AddOrUpdate(revision.RevisionId, offset);
    else
        dumpRef->revisionIdIndex->Add(revision.RevisionId, offset);
}

uint32_t DumpRevision::NewLength()
{
    bool ignored;
    GetModelFormatId(ignored);

    if (withText && !textUnloaded && !HasFlag(revision.Flags, RevisionFlags::TextDeleted))
        textLength = revision.GetCompressedText().length();

    return ValueSize(DumpObjectKind::Revision) + LengthCore(revision, modelFormatId, withText, textLength);
}

DumpRevision::DumpRevision(std::weak_ptr<WritableDump> dump, std::uint32_t revisionId, bool loadText)
    : DumpObject(dump), revision(), modelFormatId(),
        isModelFormatIdNew(false), wasLoaded(true), textUnloaded(false), diffWriter(), forceDiff(false)
{
    withText = IsPages(dump.lock()->fileHeader.Kind);
    Load(revisionId, loadText);
}

std::uint8_t DumpRevision::GetModelFormatId(bool &isNew)
{
    if (modelFormatId == 0 && !HasFlag(revision.Flags, RevisionFlags::WikitextModelFormat))
    {
        modelFormatId = dump.lock()->GetIdForModelFormat(revision.Model, revision.Format, isModelFormatIdNew);
        if (modelFormatId == 0)
            revision.Flags |= RevisionFlags::WikitextModelFormat;
    }

    isNew = isModelFormatIdNew;
    return modelFormatId;
}

void DumpRevision::SetModelFormatId(std::uint8_t modelFormatId)
{
    this->modelFormatId = modelFormatId;
}

Revision DumpRevision::ReadCore(std::istream &stream, std::uint8_t &modelFormatId, bool withText, bool loadText)
{
    Revision revision;

    revision.RevisionId = DumpTraits<uint32_t>::Read(stream);
    revision.Flags = (RevisionFlags)DumpTraits<uint8_t>::Read(stream);
    revision.ParentId = DumpTraits<uint32_t>::Read(stream);
    revision.DateTime = DumpTraits<uint32_t>::Read(stream);
    if (!HasFlag(revision.Flags, RevisionFlags::ContributorDeleted))
        revision.Contributor = DumpUser::Read(revision.Flags, stream)->GetUser();
    if (!HasFlag(revision.Flags, RevisionFlags::CommentDeleted))
        revision.Comment = DumpTraits<string>::Read(stream);

    if (HasFlag(revision.Flags, RevisionFlags::WikitextModelFormat))
        modelFormatId = 0;
    else
        modelFormatId = DumpTraits<std::uint8_t>::Read(stream);

    if (!HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        revision.Sha1 = DumpTraits<std::string>::Read(stream);

        if (withText)
        {
            if (loadText)
            {
                std::string compressedText = DumpTraits<string>::ReadLong(stream);
                revision.SetCompressedText(compressedText);
            }
        }
        else
        {
            ReadValue(stream, revision.TextLength);
        }
    }

    return revision;
}

void DumpRevision::WriteCore(std::ostream &stream, Revision &revision, std::uint8_t modelFormatId, bool withText)
{
    WriteValue(stream, revision.RevisionId);
    WriteValue(stream, revision.Flags);
    WriteValue(stream, revision.ParentId);
    WriteValue(stream, revision.DateTime.ToInteger());
    if (!HasFlag(revision.Flags, RevisionFlags::ContributorDeleted))
        DumpUser::Create(revision.Contributor)->Write(&stream);
    if (!HasFlag(revision.Flags, RevisionFlags::CommentDeleted))
        WriteValue(stream, revision.Comment);

    if (modelFormatId != 0)
        WriteValue(stream, modelFormatId);
    
    if (!HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        // TODO: convert from base36 for saving
        WriteValue(stream, revision.Sha1);

        if (withText)
            DumpTraits<std::string>::WriteLong(stream, revision.GetCompressedText());
        else
            WriteValue(stream, revision.TextLength);
    }
}

std::uint32_t DumpRevision::LengthCore(const Revision &revision, std::uint8_t modelFormatId, bool withText, std::uint32_t textLength)
{
    uint32_t result = 0;

    result += ValueSize(revision.RevisionId);
    result += ValueSize(revision.Flags);
    result += ValueSize(revision.ParentId);
    result += ValueSize(revision.DateTime.ToInteger());
    if (!HasFlag(revision.Flags, RevisionFlags::ContributorDeleted))
        result += DumpUser::Create(revision.Contributor)->NewLength();
    if (!HasFlag(revision.Flags, RevisionFlags::CommentDeleted))
        result += ValueSize(revision.Comment);
    if (modelFormatId != 0)
        result += ValueSize(modelFormatId);
    if (!HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        result += ValueSize(revision.Sha1);

        if (withText)
            result += ValueSize(textLength) + textLength;
        else
            result += ValueSize(revision.TextLength);
    }

    return result;
}
