#include "Revision.h"
#include "../DumpException.h"

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

    if (getTextFunction == nullptr)
        throw DumpException();

    SetText(getTextFunction());

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