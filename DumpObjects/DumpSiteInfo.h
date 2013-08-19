#pragma once

#include "DumpObject.h"
#include "../Objects/SiteInfo.h"

class DumpSiteInfo : public DumpObject
{
private:
    static SiteInfo Read(std::shared_ptr<WritableDump> dump, Offset offset);
protected:
    virtual void WriteInternal() override;
    virtual void UpdateIndex(Offset offset, bool overwrite) override;
public:
    DumpSiteInfo(std::weak_ptr<WritableDump> dump);

    SiteInfo siteInfo;

    virtual uint32_t NewLength() override;

    static SiteInfo ReadCore(std::istream &stream);
    static void WriteCore(std::ostream &stream, const SiteInfo &siteInfo);
    static std::uint32_t LengthCore(const SiteInfo &siteInfo);
};