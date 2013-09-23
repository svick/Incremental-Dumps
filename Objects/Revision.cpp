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
    : Flags(), RevisionId(), TextId(), ParentId(), DateTime(), Contributor(), Comment(), textSet(false)
{}

std::string Revision::GetText()
{
    if (textSet)
        return text;

    if (getTextFunction != nullptr)
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
}

void Revision::SetGetText(std::function<std::string()> getTextFunction)
{
    this->getTextFunction = getTextFunction;
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