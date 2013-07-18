#include "Revision.h"

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
    : Flags(), RevisionId(), ParentId(), DateTime(), Contributor(), Comment(), Text()
{}
