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
    revision.Contributor = DumpUser::Read(revision.Flags, stream)->GetUser();
    revision.Comment = DumpTraits<string>::Read(stream);
    string compressedText = DumpTraits<string>::ReadLong(stream);
    revision.Text = SevenZip::Decompress(compressedText);

    return revision;
}

void DumpRevision::EnsureCompressed()
{
    if (withText && !revision.Text.empty() && compressedText.empty())
        compressedText = SevenZip::Compress(revision.Text);
}

void DumpRevision::WriteInternal()
{
    auto user = DumpUser::Create(revision.Contributor);
    EnsureCompressed();

    WriteValue((uint8_t)DumpObjectKind::Revision);
    WriteValue(revision.RevisionId);
    WriteValue((uint8_t)revision.Flags);
    WriteValue(revision.ParentId);
    WriteValue(revision.DateTime.ToInteger());
    user->Write(stream);
    WriteValue(revision.Comment);
    DumpTraits<string>::WriteLong(*stream, withText ? compressedText : string());
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
    auto user = DumpUser::Create(revision.Contributor);
    EnsureCompressed();

    return ValueSize((uint8_t)DumpObjectKind::Revision)
        + ValueSize(revision.RevisionId)
        + ValueSize((uint8_t)revision.Flags)
        + ValueSize(revision.ParentId)
        + ValueSize(revision.DateTime.ToInteger())
        + user->NewLength()
        + ValueSize(revision.Comment)
        + DumpTraits<string>::DumpSizeLong(withText ? compressedText : string());
}

DumpRevision::DumpRevision(weak_ptr<WritableDump> dump, uint32_t revisionId, bool withText)
    : DumpObject(dump), revision(), withText(withText)
{
    Load(revisionId);
}

DumpRevision::DumpRevision(weak_ptr<WritableDump> dump, bool withText)
    : DumpObject(dump), revision(), withText(withText)
{}
