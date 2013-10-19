#pragma once

#include <iostream>
#include "DumpObject.h"
#include "Offset.h"
#include "../DumpKind.h"

class Dump;

/**
 * Contains basic information about the file.
 * On disk, this object is located right at the start of the file.
 */
class FileHeader : public DumpObject
{
private:
    static std::uint32_t Length();

    FileHeader(
        DumpKind kind,
        Offset fileEnd, Offset pageIdIndexRoot, Offset revisionIdIndexRoot,
        Offset textGroupIdIndexRoot, Offset modelFormatIndexRoot,
        Offset freeSpaceIndexRoot, Offset siteInfo, std::weak_ptr<Dump> dump);
protected:
    void WriteInternal();
public:
    /**
     * Magic number that identifies dump report files.
     */
    static const std::string MagicNumber;
    /**
     * The current version of the format.
     */
    static const std::uint8_t FileFormatVersion;
    /**
     * The current version of the data schema.
     */
    static const std::uint8_t FileDataVersion;

    /**
     * Reads file header from the given @a dump.
     */
    static FileHeader Read(Dump const &dump);

    virtual void Write() OVERRIDE;
    virtual std::uint32_t NewLength() OVERRIDE;

    /**
     * The kind of the dump.
     */
    DumpKind Kind;

    /**
     * Offset to the end of the dump file.
     * In other words, the length of the whole file.
     */
    Offset FileEnd;
    /**
     * Offset to the root node object for the page ID index.
     */
    Offset PageIdIndexRoot;
    /**
     * Offset to the root node object for the revision ID index.
     */
    Offset RevisionIdIndexRoot;
    /**
     * Offset to the root node object for the text group ID index.
     */
    Offset TextGroupIdIndexRoot;
    /**
     * Offset to the root node object for the model format index.
     */
    Offset ModelFormatIndexRoot;
    /**
     * Offset to the root node object for the free space index.
     */
    Offset FreeSpaceIndexRoot;
    /**
     * Offset to the site info object.
     */
    Offset SiteInfo;

    FileHeader(std::weak_ptr<Dump> dump = std::weak_ptr<Dump>());
};
