#pragma once

#include "Change.h"
#include "../../Objects/SiteInfo.h"

/**
 * A change that contains updated SiteInfo, plus some additional data.
 *
 * This change is always present in the dump, even if SiteInfo didn't change.
 */
class SiteInfoChange : public Change
{
public:
    /**
     * The updated SiteInfo.
     */
    SiteInfo siteInfo;

    /**
     * The name of the wiki this diff dump should be applied to.
     */
    std::string name;
    /**
     * The timestamp of the dump this diff dump should be applied to.
     */
    std::string oldTimestamp;
    /**
     * The timestamp the dump should be updated to by applying this diff dump.
     */
    std::string newTimestamp;

    SiteInfoChange(const SiteInfo &siteInfo, const std::string &name, const std::string &oldTimestamp, const std::string &newTimestamp)
        : siteInfo(siteInfo), name(name), oldTimestamp(oldTimestamp), newTimestamp(newTimestamp)
    {}

    virtual void WriteInternal() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;

    static SiteInfoChange Read(std::istream &stream);
};