#pragma once

#include <cstdint>

enum class DumpKind : std::uint8_t
{
    None     = 0x00,
    Pages    = 0x01,
    Current  = 0x02,
    Articles = 0x04
};

bool IsPages(DumpKind dumpKind);
bool IsStub(DumpKind dumpKind);

bool IsCurrent(DumpKind dumpKind);
bool IsHistory(DumpKind dumpKind);

bool IsArticles(DumpKind dumpKind);

DumpKind operator |(DumpKind first, DumpKind second);
DumpKind operator |=(DumpKind &first, DumpKind second);