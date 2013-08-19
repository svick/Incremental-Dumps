#pragma once

#include "DumpObject.h"
#include "../Dump.h"
#include "../Objects/Page.h"
#include "../Diff/DiffWriter.h"

class DumpPage : public DumpObject
{
private:
    Page originalPage;
    bool wasLoaded;

    DiffWriter *diffWriter;

    void Load(std::uint32_t pageId);
    static Page Read(std::shared_ptr<WritableDump> dump, Offset offset);
protected:
    virtual void WriteInternal() override;
    virtual void UpdateIndex(Offset offset, bool overwrite) override;
public:
    Page page;

    DumpPage(std::weak_ptr<WritableDump> dump, std::uint32_t pageId);
    DumpPage(std::weak_ptr<WritableDump> dump, Offset offset);

    virtual void Write() override;
    void Write(DiffWriter *diffWriter);
    virtual std::uint32_t NewLength() override;

    static Page ReadCore(std::istream &stream, bool includeRevisionIds);
    static void WriteCore(std::ostream &stream, const Page &page, bool includeRevisionIds);
    static std::uint32_t LengthCore(const Page &page, bool includeRevisionIds);
};