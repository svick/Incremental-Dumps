#include "DumpKind.h"

bool IsPages(DumpKind dumpKind)
{
    return ((std::uint8_t)dumpKind & (std::uint8_t)DumpKind::Pages) != 0;
}

bool IsStub(DumpKind dumpKind)
{
    return !IsPages(dumpKind);
}

bool IsCurrent(DumpKind dumpKind)
{
    return ((std::uint8_t)dumpKind & (std::uint8_t)DumpKind::Current) != 0;
}

bool IsHistory(DumpKind dumpKind)
{
    return !IsCurrent(dumpKind);
}

bool IsArticles(DumpKind dumpKind)
{
    return ((std::uint8_t)dumpKind & (std::uint8_t)DumpKind::Articles) != 0;
}

DumpKind operator |(DumpKind first, DumpKind second)
{
    return (DumpKind)((std::uint8_t)first | (std::uint8_t)second);
}

DumpKind operator |=(DumpKind &first, DumpKind second)
{
    first = first | second;
    return first;
}

std::ostream& operator<<(std::ostream& stream, DumpKind dumpKind)
{
    if (IsStub(dumpKind))
        stream << "stub";
    else
        stream << "pages";

    if (IsCurrent(dumpKind))
        stream << "-current";
    else
        stream << "-history";

    if (IsArticles(dumpKind))
        stream << "-articles";
  
    return stream;
}