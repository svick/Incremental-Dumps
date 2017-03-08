#include "DumpSiteInfo.h"
#include "DumpObjectKind.h"
#include "../Dump.h"
#include "../format.h"

void DumpSiteInfo::WriteInternal()
{
    WriteValue(DumpObjectKind::SiteInfo);

    WriteValue(name);
    WriteValue(timestamp);

    WriteCore(*stream, siteInfo);
}

void DumpSiteInfo::UpdateIndex(bool overwrite)
{
    auto dumpRef = dump.lock();

    dumpRef->fileHeader.SiteInfo = savedOffset;
    dumpRef->fileHeader.Write();
}

DumpSiteInfo::DumpSiteInfo(std::weak_ptr<Dump> dump)
    : DumpObject(dump)
{
    auto dumpRef = dump.lock();
    auto siteInfoOffset = dumpRef->fileHeader.SiteInfo;
    if (siteInfoOffset.value == 0)
    {
        savedOffset = 0;
        savedLength = 0;
    }
    else
    {
        Read(dumpRef, siteInfoOffset);
        savedOffset = siteInfoOffset.value;
        savedLength = NewLength();
    }
}

void DumpSiteInfo::Read(std::shared_ptr<Dump> dump, Offset offset)
{
    if (offset.value == 0)
        return;

    auto &stream = *(dump->stream);
    stream.seekp(offset.value);

    DumpObjectKind kind;
    ReadValue(stream, kind);
    if (kind != DumpObjectKind::SiteInfo)
        throw new DumpException();

    ReadValue(stream, name);
    ReadValue(stream, timestamp);

    siteInfo = ReadCore(stream);
}

std::uint32_t DumpSiteInfo::NewLength()
{
    uint32_t length = ValueSize(DumpObjectKind::SiteInfo) + ValueSize(name) + ValueSize(timestamp) + LengthCore(siteInfo);

    // allocate more than necessary, so that reallocation isn't required when site info changes slightly
    const uint32_t rounding = 100;

    return (length + rounding - 1) / rounding * rounding;
}

void DumpSiteInfo::CheckName(const std::string &name, bool canBeEmpty) const
{
    if (this->name == name)
        return;

    if (canBeEmpty && this->name.empty())
        return;

    throw UserException(
        str(fmt::Format("The name of the dump is {0}, but expected {1}.") << this->name << name));
}

void DumpSiteInfo::CheckTimestamp(const std::string &timestamp, bool shouldBeDifferent) const
{
    if (shouldBeDifferent)
    {
        if (this->timestamp == timestamp)
            throw UserException(
                str(fmt::Format("Both old and new timestamps are {0}, but they have to be different.") << timestamp));
    }
    else
    {
        if (this->timestamp != timestamp)
            throw UserException(
                str(fmt::Format("The timestamp of the dump is {0}, but expected {1}.") << this->timestamp << timestamp));
    }
}

SiteInfo DumpSiteInfo::ReadCore(std::istream &stream)
{
    SiteInfo siteInfo;

    ReadValue(stream, siteInfo.Lang);
    ReadValue(stream, siteInfo.SiteName);
    ReadValue(stream, siteInfo.DbName);
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
    WriteValue(stream, siteInfo.DbName);
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
        + ValueSize(siteInfo.DbName)
        + ValueSize(siteInfo.Base)
        + ValueSize(siteInfo.Generator)
        + ValueSize(siteInfo.SiteCase)
        + ValueSize(siteInfo.Namespaces);
}