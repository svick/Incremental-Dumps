#pragma once

#include "Change.h"
#include "../../Objects/SiteInfo.h"

class SiteInfoChange : public Change
{
public:
    SiteInfo siteInfo;

    std::string name;
    std::string oldTimestamp;
    std::string newTimestamp;

    SiteInfoChange(const SiteInfo &siteInfo, const std::string &name, const std::string &oldTimestamp, const std::string &newTimestamp)
        : siteInfo(siteInfo), name(name), oldTimestamp(oldTimestamp), newTimestamp(newTimestamp)
    {}

    virtual void WriteInternal() override;
    virtual std::uint32_t NewLength() override;

    static SiteInfoChange Read(std::istream &stream);

    virtual void Accept(ChangeVisitor &visitor) override;
};