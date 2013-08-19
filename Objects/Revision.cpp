#include "Revision.h"
#include "../DumpException.h"
#include "../SevenZip.h"

RevisionFlags operator |(RevisionFlags first, RevisionFlags second)
{
    return (RevisionFlags)((uint8_t)first | (uint8_t)second);
}

RevisionFlags operator |=(RevisionFlags &first, RevisionFlags second)
{
    first = first | second;
    return first;
}

bool HasFlag(RevisionFlags value, RevisionFlags flag)
{
    return ((uint8_t)value & (uint8_t)flag) != 0;
}

Revision::Revision()
    : Flags(), RevisionId(), TextId(), ParentId(), DateTime(), Contributor(), Comment(), textSet(false), compressedTextSet(false)
{}

std::string Revision::GetText(bool canUseCompressed)
{
    if (textSet)
        return text;

    if (canUseCompressed && compressedTextSet)
        SetText(SevenZip::Decompress(GetCompressedText(false)));
    else if (getTextFunction != nullptr)
        SetText(getTextFunction());
    else
        throw DumpException();

    return text;
}

void Revision::SetText(const std::string &text)
{
    this->text = text;
    this->TextLength = text.length();
    this->textSet = true;

    this->compressedText = std::string();
    this->compressedTextSet = false;
}

// doesn't override compressed text (if set)
void Revision::SetGetText(std::function<std::string()> getTextFunction)
{
    this->getTextFunction = getTextFunction;
}

std::string Revision::GetCompressedText(bool canUseDecompressed)
{
    if (compressedTextSet)
        return compressedText;

    if (canUseDecompressed)
    {
        SetCompressedText(SevenZip::Compress(GetText(false)));
        return compressedText;
    }

    throw DumpException();
}

void Revision::SetCompressedText(const std::string &compressedText)
{
    this->compressedText = compressedText;
    this->compressedTextSet = true;
}

bool operator ==(const Revision &first, const Revision &second)
{
    return first.Flags == second.Flags
        && first.RevisionId == second.RevisionId
        && first.ParentId == second.ParentId
        && first.DateTime == second.DateTime
        && Equals(first.Contributor.get(), second.Contributor.get())
        && first.Comment == second.Comment
        && first.Sha1 == second.Sha1
        && first.Model == second.Model
        && first.Format == second.Format;
}