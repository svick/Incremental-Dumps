#include "DumpSiteInfo.h"
#include "DumpObjectKind.h"
#include "../Dump.h"

void DumpSiteInfo::WriteInternal()
{
    WriteValue(DumpObjectKind::SiteInfo);
    WriteValue(siteInfo.Lang);
    WriteValue(siteInfo.SiteName);
    WriteValue(siteInfo.Base);
    WriteValue(siteInfo.Generator);
    WriteValue(siteInfo.SiteCase);
    WriteValue(siteInfo.Namespaces);
}

void DumpSiteInfo::UpdateIndex(Offset offset, bool overwrite)
{
    dump.lock()->fileHeader.SiteInfo = offset;
}

DumpSiteInfo::DumpSiteInfo(std::weak_ptr<WritableDump> dump)
    : DumpObject(dump)
{
    auto dumpRef = dump.lock();
    auto siteInfoOffset = dumpRef->fileHeader.SiteInfo;
    if (siteInfoOffset.value == 0)
    {
        siteInfo = SiteInfo();
        savedOffset = 0;
        savedLength = 0;
    }
    else
    {
        siteInfo = Read(dumpRef, siteInfoOffset);
        savedOffset = siteInfoOffset.value;
        savedLength = NewLength();
    }
}

SiteInfo DumpSiteInfo::Read(std::shared_ptr<WritableDump> dump, Offset offset)
{
    SiteInfo siteInfo;

    if (offset.value == 0)
        return siteInfo;

    auto &stream = *(dump->stream);
    stream.seekp(offset.value);

    DumpObjectKind kind;
    ReadValue(stream, kind);
    if (kind != DumpObjectKind::SiteInfo)
        throw new DumpException();

    ReadValue(stream, siteInfo.Lang);
    ReadValue(stream, siteInfo.SiteName);
    ReadValue(stream, siteInfo.Base);
    ReadValue(stream, siteInfo.Generator);
    ReadValue(stream, siteInfo.SiteCase);
    ReadValue(stream, siteInfo.Namespaces);

    return siteInfo;
}

std::uint32_t DumpSiteInfo::NewLength()
{
    uint32_t length =
        ValueSize(DumpObjectKind::SiteInfo)
        + ValueSize(siteInfo.Lang)
        + ValueSize(siteInfo.SiteName)
        + ValueSize(siteInfo.Base)
        + ValueSize(siteInfo.Generator)
        + ValueSize(siteInfo.SiteCase)
        + ValueSize(siteInfo.Namespaces);

    // allocate more than necessary, so that reallocation isn't necessary when site info changes slightly
    const uint32_t rounding = 100;

    return (length + rounding - 1) / rounding * rounding;
}