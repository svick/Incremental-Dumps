#include "SiteInfoChange.h"
#include "../../DumpObjects/DumpSiteInfo.h"

void SiteInfoChange::WriteInternal()
{
    WriteValue(ChangeKind::SiteInfo);

    WriteValue(name);
    WriteValue(oldTimestamp);
    WriteValue(newTimestamp);

    DumpSiteInfo::WriteCore(*stream, siteInfo);
}

std::uint32_t SiteInfoChange::NewLength()
{
    return ValueSize(ChangeKind::SiteInfo)
        + ValueSize(name)
        + ValueSize(oldTimestamp)
        + ValueSize(newTimestamp)
        + DumpSiteInfo::LengthCore(siteInfo);
}

SiteInfoChange SiteInfoChange::Read(std::istream &stream)
{
    std::string name, oldTimestamp, newTimestamp;

    ReadValue(stream, name);
    ReadValue(stream, oldTimestamp);
    ReadValue(stream, newTimestamp);

    SiteInfo siteInfo = DumpSiteInfo::ReadCore(stream);
    
    return SiteInfoChange(siteInfo, name, oldTimestamp, newTimestamp);
}