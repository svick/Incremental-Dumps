#include "RevisionChange.h"
#include "../ChangeVisitor.h"
#include "../../DumpObjects/DumpUser.h"
#include "../../SevenZip.h"

RevisionChangeFlags operator &(RevisionChangeFlags first, RevisionChangeFlags second)
{
    return (RevisionChangeFlags)((std::uint8_t)first & (std::uint8_t)second);
}

RevisionChangeFlags operator |(RevisionChangeFlags first, RevisionChangeFlags second)
{
    return (RevisionChangeFlags)((std::uint8_t)first | (std::uint8_t)second);
}

RevisionChangeFlags operator |=(RevisionChangeFlags &first, RevisionChangeFlags second)
{
    first = first | second;
    return first;
}

bool HasFlag(RevisionChangeFlags value, RevisionChangeFlags flag)
{
    return ((std::uint8_t)value & (std::uint8_t)flag) != 0;
}


bool flagChangedToUnset(const Revision &oldRevision, const Revision &newRevision, RevisionFlags flag)
{
    return HasFlag(oldRevision.Flags, flag) && !HasFlag(newRevision.Flags, flag);
}


void RevisionChange::EnsureCompressed()
{
    if (compressedTextSet)
        return;

    compressedText = SevenZip::Compress(revisionChanges.GetText());
    compressedTextSet = true;
}

RevisionChange::RevisionChange(bool withText)
    : withText(withText)
{}

RevisionChange::RevisionChange(
    const Revision &oldRevision, Revision &newRevision,
    std::uint8_t newRevisionModelFormatId, bool withText)
    : withText(withText)
{
    revisionChanges = newRevision;
    this->newRevisionModelFormatId = newRevisionModelFormatId;

    flags = RevisionChangeFlags::NoChanges;

    if (oldRevision.Flags != newRevision.Flags)
        flags |= RevisionChangeFlags::Flags; // can be also set because of Contributor
    if (oldRevision.ParentId != newRevision.ParentId)
        flags |= RevisionChangeFlags::ParentId;
    if (oldRevision.DateTime != newRevision.DateTime)
        flags |= RevisionChangeFlags::DateTime;
    if (flagChangedToUnset(oldRevision, newRevision, RevisionFlags::ContributorDeleted)
        || !Equals(oldRevision.Contributor.get(), newRevision.Contributor.get()))
    {
        if (!HasFlag(newRevision.Flags, RevisionFlags::ContributorDeleted))
        {
            flags |= RevisionChangeFlags::Flags;
            flags |= RevisionChangeFlags::Contributor;
        }
    }
    if (flagChangedToUnset(oldRevision, newRevision, RevisionFlags::CommentDeleted)
        || oldRevision.Comment != newRevision.Comment)
    {
        if (!HasFlag(newRevision.Flags, RevisionFlags::CommentDeleted))
            flags |= RevisionChangeFlags::Comment;
    }
    if (flagChangedToUnset(oldRevision, newRevision, RevisionFlags::TextDeleted)
        || oldRevision.Sha1 != newRevision.Sha1)
    {
        if (!HasFlag(newRevision.Flags, RevisionFlags::TextDeleted))
            flags |= RevisionChangeFlags::Text;
    }
    if (oldRevision.Model != newRevision.Model
        || oldRevision.Format != newRevision.Format)
        flags |= RevisionChangeFlags::ModelFormat;
}

RevisionChange RevisionChange::Read(std::istream &stream, bool withText)
{
    RevisionChange result(withText);

    ReadValue(stream, result.revisionChanges.RevisionId);
    ReadValue(stream, result.flags);

    if (HasFlag(result.flags, RevisionChangeFlags::Flags))
        ReadValue(stream, result.revisionChanges.Flags);

    if (HasFlag(result.flags, RevisionChangeFlags::ParentId))
        ReadValue(stream, result.revisionChanges.ParentId);

    if (HasFlag(result.flags, RevisionChangeFlags::DateTime))
    {
        std::uint32_t dateTimeInteger;
        ReadValue(stream, dateTimeInteger);
        result.revisionChanges.DateTime = Timestamp(dateTimeInteger);
    }

    if (HasFlag(result.flags, RevisionChangeFlags::Contributor))
        result.revisionChanges.Contributor = DumpUser::Read(result.revisionChanges.Flags, stream)->GetUser();

    if (HasFlag(result.flags, RevisionChangeFlags::Comment))
        ReadValue(stream, result.revisionChanges.Comment);

    if (HasFlag(result.flags, RevisionChangeFlags::Text))
    {
        ReadValue(stream, result.revisionChanges.Sha1);

        if (withText)
        {
            auto compressedText = DumpTraits<std::string>::ReadLong(stream);
            result.revisionChanges.SetText(SevenZip::Decompress(compressedText));
        }
        else
            ReadValue(stream, result.revisionChanges.TextLength);
    }

    if (HasFlag(result.flags, RevisionChangeFlags::ModelFormat))
        ReadValue(stream, result.newRevisionModelFormatId);

    return result;
}

void RevisionChange::WriteInternal()
{
    WriteValue(ChangeKind::ChangeRevision);
    WriteValue(revisionChanges.RevisionId);
    WriteValue(flags);

    if (HasFlag(flags, RevisionChangeFlags::Flags))
        WriteValue(revisionChanges.Flags);

    if (HasFlag(flags, RevisionChangeFlags::ParentId))
        WriteValue(revisionChanges.ParentId);

    if (HasFlag(flags, RevisionChangeFlags::DateTime))
        WriteValue(revisionChanges.DateTime.ToInteger());

    if (HasFlag(flags, RevisionChangeFlags::Contributor))
        DumpUser::Create(revisionChanges.Contributor)->Write(stream);

    if (HasFlag(flags, RevisionChangeFlags::Comment))
        WriteValue(revisionChanges.Comment);

    if (HasFlag(flags, RevisionChangeFlags::Text))
    {
        WriteValue(revisionChanges.Sha1);

        if (withText)
        {
            EnsureCompressed();
            DumpTraits<std::string>::WriteLong(*stream, compressedText);
        }
        else
            WriteValue(revisionChanges.TextLength);
    }

    if (HasFlag(flags, RevisionChangeFlags::ModelFormat))
        WriteValue(newRevisionModelFormatId);
}

std::uint32_t RevisionChange::NewLength()
{
    std::uint32_t result = 0;

    result += ValueSize(ChangeKind::ChangeRevision);
    result += ValueSize(revisionChanges.RevisionId);
    result += ValueSize(flags);

    if (HasFlag(flags, RevisionChangeFlags::Flags))
        result += ValueSize(revisionChanges.Flags);

    if (HasFlag(flags, RevisionChangeFlags::ParentId))
        result += ValueSize(revisionChanges.ParentId);

    if (HasFlag(flags, RevisionChangeFlags::DateTime))
        result += ValueSize(revisionChanges.DateTime.ToInteger());

    if (HasFlag(flags, RevisionChangeFlags::Contributor))
        result += DumpUser::Create(revisionChanges.Contributor)->NewLength();

    if (HasFlag(flags, RevisionChangeFlags::Comment))
        result += ValueSize(revisionChanges.Comment);

    if (HasFlag(flags, RevisionChangeFlags::Text))
    {
        result += ValueSize(revisionChanges.Sha1);

        if (withText)
        {
            EnsureCompressed();
            result += DumpTraits<std::string>::DumpSizeLong(compressedText);
        }
        else
            result += ValueSize(revisionChanges.TextLength);
    }

    if (HasFlag(flags, RevisionChangeFlags::ModelFormat))
        result += ValueSize(newRevisionModelFormatId);

    return result;
}

void RevisionChange::Accept(ChangeVisitor &visitor)
{
    visitor.Visit(*this);
}