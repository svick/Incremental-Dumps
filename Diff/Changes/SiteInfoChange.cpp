#include "SiteInfoChange.h"
#include "../ChangeVisitor.h"
#include "../../DumpObjects/DumpSiteInfo.h"

void SiteInfoChange::WriteInternal()
{
    WriteValue(ChangeKind::SiteInfo);
    DumpSiteInfo::WriteCore(*stream, siteInfo);
}

std::uint32_t SiteInfoChange::NewLength()
{
    return ValueSize(ChangeKind::SiteInfo) + DumpSiteInfo::LengthCore(siteInfo);
}

SiteInfoChange SiteInfoChange::Read(std::istream &stream)
{
    return SiteInfoChange(DumpSiteInfo::ReadCore(stream));
}

void SiteInfoChange::Accept(ChangeVisitor &visitor)
{
    visitor.Visit(*this);
}