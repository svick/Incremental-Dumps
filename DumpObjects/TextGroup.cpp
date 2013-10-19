#include "TextGroup.h"
#include "../Indexes/Index.h"
#include "../SevenZip.h"
#include "../StringHelpers.h"

// UTF-8 encoded U+FFFF Unicode Noncharacter
const std::string deletedTextMark = "\xEF\xBF\xBF";

void TextGroup::EnsureCompressed()
{
    if (!compressedTexts.empty())
        return;

    std::string textsString;

    for (auto &text : texts)
    {
        textsString.append(text);
        textsString.append(1, '\0');
    }

    compressedTexts = SevenZip::Compress(textsString);
}

void TextGroup::Load()
{
    auto dumpRef = dump.lock();
    auto revisionOffset = dumpRef->textGroupIdIndex->Get(textGroupId);
    if (revisionOffset.value == 0)
    {
        savedOffset = 0;
        savedLength = 0;
        isEditable = true;
    }
    else
    {
        texts = Read(revisionOffset, savedLength);
        savedOffset = revisionOffset.value;
        isEditable = false;
    }
}

std::vector<std::string> TextGroup::Read(Offset offset, std::uint32_t& length)
{
    auto &stream = *(dump.lock()->stream);
    stream.seekp(offset.value);

    auto kind = DumpTraits<DumpObjectKind>::Read(stream);
    if (kind != DumpObjectKind::TextGroup)
        throw new DumpException();

    auto textGroupId = DumpTraits<std::uint32_t>::Read(stream);
    if (textGroupId != this->textGroupId)
        throw new DumpException();

    auto compressed = DumpTraits<std::string>::ReadLong(stream);
    auto textsString = SevenZip::Decompress(compressed);

    length = (std::uint64_t)stream.tellp() - offset.value;

    return split(textsString, '\0');
}

void TextGroup::WriteInternal()
{
    EnsureCompressed();

    WriteValue(DumpObjectKind::TextGroup);
    WriteValue(textGroupId);
    DumpTraits<std::string>::WriteLong(*stream, compressedTexts);
}

void TextGroup::UpdateIndex(bool overwrite)
{
    auto dumpRef = dump.lock();

    if (overwrite)
        dumpRef->textGroupIdIndex->AddOrUpdate(textGroupId, savedOffset);
    else
        dumpRef->textGroupIdIndex->Add(textGroupId, savedOffset);
}

TextGroup::TextGroup(std::weak_ptr<Dump> dump, std::uint32_t textGroupId)
    : DumpObject(dump), textGroupId(textGroupId)
{
    Load();
}

uint32_t TextGroup::NewLength()
{
    EnsureCompressed();

    return ValueSize(DumpObjectKind::TextGroup) + ValueSize(textGroupId) + DumpTraits<std::string>::DumpSizeLong(compressedTexts);
}

std::uint32_t TextGroup::GetTextGroupId() const
{
    return textGroupId;
}

std::uint8_t TextGroup::AddText(const std::string& text)
{
    if (!isEditable)
        throw DumpException();

    if (texts.size() > 0xFF)
        throw DumpException();

#if _DEBUG
    // text can't contain 0 bytes
    if (text.find('\0') != std::string::npos)
        throw DumpException();

    // text can't be just U+FFFF
    if (text == deletedTextMark)
        throw DumpException();
#endif

    texts.push_back(text);

    return texts.size() - 1;
}

bool TextGroup::DeleteText(std::uint8_t textId)
{
    texts.at(textId) = deletedTextMark;
    compressedTexts.clear();

    if (IsEmpty())
    {
        auto dumpRef = dump.lock();

        dumpRef->textGroupIdIndex->Remove(textGroupId);
        dumpRef->spaceManager->Delete(savedOffset, savedLength);

        return true;
    }

    return false;
}

std::string TextGroup::GetText(std::uint8_t textId) const
{
    auto result = texts.at(textId);

    if (result == deletedTextMark)
        throw DumpException();

    return result;
}

void TextGroup::SetCompressedTexts(const std::string& newCompressedTexts)
{
    if (!compressedTexts.empty() || !texts.empty())
        throw DumpException();

    compressedTexts = newCompressedTexts;
}

const std::string& TextGroup::GetCompressedTexts() const
{
    return compressedTexts;
}

bool TextGroup::IsEmpty() const
{
    return std::all_of(texts.begin(), texts.end(), [](const std::string &s) { return s == deletedTextMark; });
}

bool TextGroup::IsFull() const
{
    return texts.size() == 0x100;
}