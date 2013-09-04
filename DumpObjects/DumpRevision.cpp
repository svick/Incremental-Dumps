#include "DumpRevision.h"
#include "DumpObjectKind.h"
#include "DumpUser.h"
#include "../SevenZip.h"
#include "../Indexes/Index.h"
#include "../TextGroupsManager.h"

void DumpRevision::Load(std::uint32_t revisionId)
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
        revision = Read(dumpRef, revisionOffset);

        originalRevision = revision;

        savedOffset = revisionOffset.value;
        savedLength = NewLength();
    }
}

Revision DumpRevision::Read(std::shared_ptr<WritableDump> dump, Offset offset)
{
    auto &stream = *(dump->stream);
    stream.seekp(offset.value);

    auto kind = DumpTraits<DumpObjectKind>::Read(stream);
    if (kind != DumpObjectKind::Revision)
        throw new DumpException();

    std::uint8_t modelFormatId;
    auto revision = ReadCore(stream, modelFormatId, withText);

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

    if (!HasFlag(revision.Flags, RevisionFlags::TextDeleted) && withText)
    {
        ReadValue(stream, textGroupId);
        ReadValue(stream, textId);

        revision.SetGetText([this](){ return this->dump.lock()->textGroupsManager->GetTextFromGroup(textGroupId, textId); });
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

    if (withText && !HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        if (textGroupId == 0 || originalRevision.Sha1 != revision.Sha1)
        {
            if (textGroupId != 0)
                DeleteText();

            auto textLocation = dump.lock()->textGroupsManager->AddTextToGroup(revision.GetText());
            textGroupId = textLocation.first;
            textId = textLocation.second;
        }

        WriteValue(textGroupId);
        WriteValue(textId);
    }

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

    dumpRef->textGroupsManager->WriteTextGroupIfFull();
}

std::uint32_t DumpRevision::NewLength()
{
    bool ignored;
    GetModelFormatId(ignored);

    auto result = ValueSize(DumpObjectKind::Revision) + LengthCore(revision, modelFormatId, withText);

    if (withText)
        result += ValueSize(textGroupId) + ValueSize(textId);

    return result;
}

DumpRevision::DumpRevision(std::weak_ptr<WritableDump> dump, std::uint32_t revisionId)
    : DumpObject(dump), revision(), modelFormatId(), textGroupId(0), textId(0),
        isModelFormatIdNew(false), wasLoaded(true), textUnloaded(false), diffWriter(), forceDiff(false)
{
    withText = IsPages(dump.lock()->fileHeader.Kind);
    Load(revisionId);
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

char getCharForDigit(std::uint8_t digit, std::uint16_t base)
{
    if (base >= 2 && base <= 36)
    {
        if (digit >= base)
            throw DumpException();

        if (digit < 10)
            return '0' + digit;

        return 'a' + (digit - 10);
    }

    if (base == 256)
    {
        return (char)digit;
    }

    throw DumpException();
}

std::uint8_t getDigitForChar(char c, std::uint16_t base)
{
    if (base >= 2 && base <= 36)
    {
        std::uint8_t result;

        if (c >= '0' && c <= '9')
            result = c - '0';
        else if (c >= 'a' && c <= 'z')
            result = 10 + (c - 'a');
        else
            throw DumpException();

        if (result >= base)
            throw DumpException();

        return result;
    }

    if (base == 256)
    {
        return (std::uint8_t)c;
    }

    throw DumpException();
}

// ported version of wfBaseConvert
std::string baseConvert(const std::string& input, std::uint16_t sourceBase, std::uint16_t destBase, std::uint8_t pad)
{
    std::string result;
    result.reserve(pad);

    std::vector<std::uint8_t> inDigits;

    std::transform(input.begin(), input.end(), std::back_inserter(inDigits), [=](char c) { return getDigitForChar(c, sourceBase); });

    while (!inDigits.empty())
    {
        std::uint16_t work = 0;
        std::vector<std::uint8_t> workDigits;

        for (std::uint8_t digit : inDigits)
        {
            work *= sourceBase;
            work += digit;

            if (!workDigits.empty() || work >= destBase)
                workDigits.push_back(work / destBase);

            work %= destBase;
        }

        result.push_back(getCharForDigit(work, destBase));

        inDigits = workDigits;
    }

    if (pad > result.length())
        result.resize(pad, getCharForDigit(0, destBase));

    std::reverse(result.begin(), result.end());

    return result;
}

const std::uint8_t rawSha1Length = 20;
const std::uint8_t base36Sha1Length = 31;

std::string convertFromBase36(const std::string& input)
{
    auto result = baseConvert(input, 36, 256, rawSha1Length);

    if (result.length() != rawSha1Length)
        throw DumpException();

    std::reverse(result.begin(), result.end());

    return result;
}

// note: modifies its input
std::string convertToBase36(std::string& input)
{
    std::reverse(input.begin(), input.end());

    auto result =  baseConvert(input, 256, 36, base36Sha1Length);

    if (result.length() != base36Sha1Length)
        throw DumpException();

    return result;
}

Revision DumpRevision::ReadCore(std::istream &stream, std::uint8_t &modelFormatId, bool withText)
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
        std::string rawSha1;
        rawSha1.reserve(rawSha1Length);

        for (int i = 0; i < rawSha1Length; i++)
            rawSha1.push_back(DumpTraits<char>::Read(stream));

        revision.Sha1 = convertToBase36(rawSha1);

        if (!withText)
            ReadValue(stream, revision.TextLength);
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
        auto convertedSha1 = convertFromBase36(revision.Sha1);
        for (int i = 0; i < rawSha1Length; i++)
            WriteValue(stream, convertedSha1[i]);

        if (!withText)
            WriteValue(stream, revision.TextLength);
    }
}

std::uint32_t DumpRevision::LengthCore(const Revision &revision, std::uint8_t modelFormatId, bool withText)
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
        result += rawSha1Length;

        if (!withText)
            result += ValueSize(revision.TextLength);
    }

    return result;
}

void DumpRevision::DeleteText()
{
    if (withText && !HasFlag(revision.Flags, RevisionFlags::TextDeleted))
    {
        dump.lock()->textGroupsManager->DeleteTextFromGroup(textGroupId, textId);
    }
}