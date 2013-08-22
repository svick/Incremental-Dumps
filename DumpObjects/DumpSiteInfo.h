#pragma once

#include "DumpObject.h"
#include "../Objects/SiteInfo.h"

class DumpSiteInfo : public DumpObject
{
private:
    void Read(std::shared_ptr<WritableDump> dump, Offset offset);
protected:
    virtual void WriteInternal() override;
    virtual void UpdateIndex(Offset offset, bool overwrite) override;
public:
    DumpSiteInfo(std::weak_ptr<WritableDump> dump);

    std::string name;
    std::string timestamp;

    SiteInfo siteInfo;

    virtual uint32_t NewLength() override;

    void CheckName(const std::string &name, bool canBeEmpty = false) const;
    // if shouldBeDifferent is false, makes sure that the timestamps are the same
    // if it's true, makes sure they are different
    void CheckTimestamp(const std::string &timestamp, bool shouldBeDifferent = false) const;

    static SiteInfo ReadCore(std::istream &stream);
    static void WriteCore(std::ostream &stream, const SiteInfo &siteInfo);
    static std::uint32_t LengthCore(const SiteInfo &siteInfo);
};