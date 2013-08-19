#pragma once

#include "Change.h"
#include "../../Objects/SiteInfo.h"

class SiteInfoChange : public Change
{
public:
    SiteInfo siteInfo;

    SiteInfoChange(const SiteInfo &siteInfo)
        : siteInfo(siteInfo)
    {}

    virtual void WriteInternal() override;
    virtual std::uint32_t NewLength() override;

    static SiteInfoChange Read(std::istream &stream);

    virtual void Accept(ChangeVisitor &visitor) override;
};