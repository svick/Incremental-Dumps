#pragma once

#include "DumpObject.h"
#include "../Dump.h"
#include "../Objects/Page.h"
#include "../Diff/DiffWriter.h"

/**
 * Represents a Page on the disk.
 */
class DumpPage : public DumpObject
{
private:
    Page originalPage;
    bool wasLoaded;

    void Load(std::uint32_t pageId);
    static Page Read(std::shared_ptr<WritableDump> dump, Offset offset);
protected:
    virtual void WriteInternal() OVERRIDE;
    virtual void UpdateIndex(bool overwrite) OVERRIDE;
public:
    /**
     * The page this object represents.
     */
    Page page;

    /**
     * Loads page based on its ID.
     */
    DumpPage(std::weak_ptr<WritableDump> dump, std::uint32_t pageId);
    /**
     * Loads page based on its offset in the dump file.
     */
    DumpPage(std::weak_ptr<WritableDump> dump, Offset offset);

    virtual void Write() OVERRIDE;
    /**
     * Writes the changes that were made to this page since it was loaded to @a diffWriter.
     */
    void WriteDiff(DiffWriter& diffWriter);
    virtual std::uint32_t NewLength() OVERRIDE;

    /**
     * Reads the "core" of the page object from @a stream and returns it.
     *
     * This excludes ::DumpObjectKind and Page::RevisionIds.
     */
    static Page ReadCore(std::istream &stream);
    /**
     * Writes the "core" of the given @a page to the @a stream.
     */
    static void WriteCore(std::ostream &stream, const Page &page);
    /**
     * Returns the length of the "core" of the given @a page.
     */
    static std::uint32_t LengthCore(const Page &page);
};