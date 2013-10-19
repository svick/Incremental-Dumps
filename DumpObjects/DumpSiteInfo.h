#pragma once

#include "DumpObject.h"
#include "../Objects/SiteInfo.h"

/**
 * Represents the SiteInfo object on the disk (along with some additional information).
 */
class DumpSiteInfo : public DumpObject
{
private:
    void Read(std::shared_ptr<Dump> dump, Offset offset);
protected:
    virtual void WriteInternal() OVERRIDE;
    virtual void UpdateIndex(bool overwrite) OVERRIDE;
public:
    DumpSiteInfo(std::weak_ptr<Dump> dump);

    /**
     * Name of the dump, used primarily to make sure diff dumps are applied correctly.
     */
    std::string name;
    /**
     * %Timestamp of the dump, used primarily to make sure diff dumps are applied correctly.
     *
     * This doesn't actually have to be timestamp, it can be any string.
     */
    std::string timestamp;

    /**
     * The site info object this object represents.
     */
    SiteInfo siteInfo;

    virtual uint32_t NewLength() OVERRIDE;

    /**
     * Checks #name against the given @a name.
     * If the check fails, throws exception.
     *
     * @param canBeEmpty If @c true and #name is empty, doesn't throw exception.
     *                   This is used for new dumps, that don't have a name yet.
     */
    void CheckName(const std::string &name, bool canBeEmpty = false) const;
    /**
    * Checks #timestamp against the given @a timestamp.
    * If the check fails, throws exception.
    *
    * @param shouldBeDifferent If it's @c false, makes sure that the timestamps are the same (used when applying diff dump).
    *                          If it's @c true, makes sure they are different (used when updating dump).
    */
    void CheckTimestamp(const std::string &timestamp, bool shouldBeDifferent) const;

    /**
     * Reads the "core" of the site info object from @a stream and returns it.
     *
     * This excludes ::DumpObjectKind, #name and #timestamp.
     */
    static SiteInfo ReadCore(std::istream &stream);
    /**
     * Writes the "core" of the given @a siteInfo to the @a stream.
     */
    static void WriteCore(std::ostream &stream, const SiteInfo &siteInfo);
    /**
     * Returns the length of the "core" of the given @a siteInfo.
     */    
    static std::uint32_t LengthCore(const SiteInfo &siteInfo);
};