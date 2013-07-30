#include "DumpRevision.h"
#include "DumpObjectKind.h"
#include "DumpUser.h"
#include "../SevenZip.h"
#include "../Indexes/Index.h"

void DumpRevision::Load(uint32_t revisionId)
{
    auto dumpRef = dump.lock();
    auto revisionOffset = dumpRef->revisionIdIndex->Get(revisionId);
    if (revisionOffset.value == 0)
    {
        revision = Revision();
        savedOffset = 0;
        savedLength = 0;
    }
    else
    {
        revision = Read(dumpRef, revisionOffset);
        savedOffset = revisionOffset.value;
        savedLength = NewLength();
    }
}

Revision DumpRevision::Read(shared_ptr<WritableDump> dump, Offset offset)
{
    Revision revision;

    auto &stream = *(dump->stream);
    stream.seekp(offset.value);

    auto kind = DumpTraits<uint8_t>::Read(stream);
    if (kind != (uint8_t)DumpObjectKind::Revision)
        throw new DumpException();

    revision.RevisionId = DumpTraits<uint32_t>::Read(stream);
    revision.Flags = (RevisionFlags)DumpTraits<uint8_t>::Read(stream);
    revision.ParentId = DumpTraits<uint32_t>::Read(stream);
    revision.DateTime = DumpTraits<uint32_t>::Read(stream);
    if (!HasFlag(revision.Flags, RevisionFlags::ContributorDeleted))
        revision.Contributor = DumpUser::Read(revision.Flags, stream)->GetUser();
    if (!HasFlag(revision.Flags, RevisionFlags::CommentDeleted))
        revision.Comment = DumpTraits<string>::Read(stream);
    revision.Sha1 = DumpTraits<std::string>::Read(stream);

    if (HasFlag(revision.Flags, RevisionFlags::WikitextModelFormat))
    {
        revision.Model = WritableDump::WikitextModel;
        revision.Format = WritableDump::WikitextFormat;
    }
    else
    {
        auto modelFormat = dump->GetModelFormat(DumpTraits<std::uint8_t>::Read(stream));
        revision.Model = modelFormat.first;
        revision.Format = modelFormat.second;
    }

    if (!HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        if (IsPages(dump->fileHeader.Kind))
        {
            std::string compressedText = DumpTraits<string>::ReadLong(stream);
            revision.Text = SevenZip::Decompress(compressedText);
            revision.TextLength = revision.Text.length();
        }
        else
        {
            ReadValue(stream, revision.TextLength);
        }
    }

    return revision;
}

void DumpRevision::EnsureCompressed()
{
    if (withText && !revision.Text.empty() && compressedText.empty())
        compressedText = SevenZip::Compress(revision.Text);
}

void DumpRevision::WriteInternal()
{
    EnsureCompressed();

    std::uint8_t modelFormatId = dump.lock()->GetIdForModelFormat(revision.Model, revision.Format);
    if (modelFormatId == 0)
        revision.Flags |= RevisionFlags::WikitextModelFormat;

    WriteValue((uint8_t)DumpObjectKind::Revision);
    WriteValue(revision.RevisionId);
    WriteValue((uint8_t)revision.Flags);
    WriteValue(revision.ParentId);
    WriteValue(revision.DateTime.ToInteger());
    if (!HasFlag(revision.Flags, RevisionFlags::ContributorDeleted))
        DumpUser::Create(revision.Contributor)->Write(stream);
    if (!HasFlag(revision.Flags, RevisionFlags::CommentDeleted))
    WriteValue(revision.Comment);
    // TODO: convert from base36 for saving
    WriteValue(revision.Sha1);

    if (modelFormatId != 0)
        WriteValue(modelFormatId);
    
    if (!HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        if (withText)
            DumpTraits<std::string>::WriteLong(*stream, compressedText);
        else
            WriteValue(revision.TextLength);
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
    EnsureCompressed();

    std::uint8_t modelFormatId = dump.lock()->GetIdForModelFormat(revision.Model, revision.Format);
    if (modelFormatId == 0)
        revision.Flags |= RevisionFlags::WikitextModelFormat;

    uint32_t result = 0;

    result += ValueSize((uint8_t)DumpObjectKind::Revision);
    result += ValueSize(revision.RevisionId);
    result += ValueSize((uint8_t)revision.Flags);
    result += ValueSize(revision.ParentId);
    result += ValueSize(revision.DateTime.ToInteger());
    if (!HasFlag(revision.Flags, RevisionFlags::ContributorDeleted))
        result += DumpUser::Create(revision.Contributor)->NewLength();
    if (!HasFlag(revision.Flags, RevisionFlags::CommentDeleted))
        result += ValueSize(revision.Comment);
    result += ValueSize(revision.Sha1);
    if (modelFormatId != 0)
        result += ValueSize(modelFormatId);
    if (!HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        if (withText)
            result += DumpTraits<string>::DumpSizeLong(compressedText);
        else
            result += ValueSize(revision.TextLength);
    }

    return result;
}

DumpRevision::DumpRevision(weak_ptr<WritableDump> dump, uint32_t revisionId, bool withText)
    : DumpObject(dump), revision(), withText(withText)
{
    Load(revisionId);
}

DumpRevision::DumpRevision(weak_ptr<WritableDump> dump, bool withText)
    : DumpObject(dump), revision(), withText(withText)
{}
