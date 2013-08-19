#include "DumpSiteInfo.h"
#include "DumpObjectKind.h"
#include "../Dump.h"

void DumpSiteInfo::WriteInternal()
{
    WriteValue(DumpObjectKind::SiteInfo);
    WriteCore(*stream, siteInfo);
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

    if (offset.value == 0)
        return SiteInfo();

    auto &stream = *(dump->stream);
    stream.seekp(offset.value);

    DumpObjectKind kind;
    ReadValue(stream, kind);
    if (kind != DumpObjectKind::SiteInfo)
        throw new DumpException();

    return ReadCore(stream);
}

std::uint32_t DumpSiteInfo::NewLength()
{
    uint32_t length = ValueSize(DumpObjectKind::SiteInfo) + LengthCore(siteInfo);

    // allocate more than necessary, so that reallocation isn't necessary when site info changes slightly
    const uint32_t rounding = 100;

    return (length + rounding - 1) / rounding * rounding;
}

SiteInfo DumpSiteInfo::ReadCore(std::istream &stream)
{
    SiteInfo siteInfo;

    ReadValue(stream, siteInfo.Lang);
    ReadValue(stream, siteInfo.SiteName);
    ReadValue(stream, siteInfo.Base);
    ReadValue(stream, siteInfo.Generator);
    ReadValue(stream, siteInfo.SiteCase);
    ReadValue(stream, siteInfo.Namespaces);

    return siteInfo;
}

void DumpSiteInfo::WriteCore(std::ostream &stream, const SiteInfo &siteInfo)
{
    WriteValue(stream, siteInfo.Lang);
    WriteValue(stream, siteInfo.SiteName);
    WriteValue(stream, siteInfo.Base);
    WriteValue(stream, siteInfo.Generator);
    WriteValue(stream, siteInfo.SiteCase);
    WriteValue(stream, siteInfo.Namespaces);
}

std::uint32_t DumpSiteInfo::LengthCore(const SiteInfo &siteInfo)
{
    return
        ValueSize(siteInfo.Lang)
        + ValueSize(siteInfo.SiteName)
        + ValueSize(siteInfo.Base)
        + ValueSize(siteInfo.Generator)
        + ValueSize(siteInfo.SiteCase)
        + ValueSize(siteInfo.Namespaces);
}