#pragma once

#include "DumpObject.h"
#include "../Dump.h"
#include "../Objects/Revision.h"
#include "../Diff/DiffWriter.h"

/**
 * Represents a Revision on the disk.
 */
class DumpRevision : public DumpObject
{
private:
    bool withText;

    Revision originalRevision;
    bool wasLoaded;

    std::uint8_t modelFormatId;
    bool isModelFormatIdNew;

    std::uint32_t textGroupId;
    std::uint8_t textId;
    /**
     * Is set when text group is set from outside
     */
    bool textSaved;

    DiffWriter *diffWriter;
    bool forceDiff;

    void Load(std::uint32_t revisionId);
    Revision Read(std::shared_ptr<Dump> dump, Offset offset);
protected:
    virtual void WriteInternal() OVERRIDE;
    virtual void UpdateIndex(bool overwrite) OVERRIDE;
public:
    /**
     * The revision this object represents.
     */
    Revision revision;

    DumpRevision(std::weak_ptr<Dump> dump, std::uint32_t revisionId);

    /**
     * Returns id for the combination of content model and format of this revision.
     *
     * @param isNew On return, this is @c true if the returned id is newly created
     */
    std::uint8_t GetModelFormatId(bool &isNew);
    /**
     * Sets the id of the combination of content model and format of this revision.
     */
    void SetModelFormatId(std::uint8_t modelFormatId);

    virtual void Write() OVERRIDE;
    /**
     * Overload of Write() that allows specifying DiffWriter.
     *
     * @param forceDiff If set to @c true, forces writing into diff, even if the revision didn't change.
     *                  This is necessary when revision moves between pages because of undeletion.
     */
    void Write(DiffWriter *diffWriter, bool forceDiff);
    virtual std::uint32_t NewLength() OVERRIDE;

    /**
     * Reads the "core" of the revision object from @a stream and returns it.
     *
     * This excludes ::DumpObjectKind and information about TextGroup where the text of this revision is located.
     *
     * @param modelFormatId Used to return the id of the combination of content model and format of the revision.
     * @param withText Whether the dump contains texts of revisions.
     */
    static Revision ReadCore(std::istream &stream, std::uint8_t &modelFormatId, bool withText);
    /**
     * Writes the "core" of the given @a revision to the @a stream.
     */
    static void WriteCore(std::ostream &stream, Revision &revision, std::uint8_t modelFormatId, bool withText);
    /**
     * Returns the length of the "core" of the given @a revision.
     */    
    static std::uint32_t LengthCore(const Revision &revision, std::uint8_t modelFormatId, bool withText);

    /**
     * Sets information about TextGroup where the text of this revision is located.
     */
    void SetTextGroup(std::uint32_t textGroupId, std::uint8_t textId);

    /**
     * Deletes the text of this revision from the dump, if there is any.
     */
    void DeleteText();
};