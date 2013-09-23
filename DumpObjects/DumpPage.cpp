#include "DumpPage.h"
#include "DumpObjectKind.h"
#include "../Indexes/Index.h"

void DumpPage::Load(uint32_t pageId)
{
    auto dumpRef = dump.lock();
    auto pageOffset = dumpRef->pageIdIndex->Get(pageId);
    if (pageOffset.value == 0)
    {
        page = Page();
        wasLoaded = false;

        savedOffset = 0;
        savedLength = 0;
    }
    else
    {
        page = Read(dumpRef, pageOffset);
        originalPage = page;
        wasLoaded = true;

        savedOffset = pageOffset.value;
        savedLength = NewLength();
    }
}

Page DumpPage::Read(shared_ptr<WritableDump> dump, Offset offset)
{
    auto &stream = *(dump->stream);
    stream.seekp(offset.value);

    auto kind = DumpTraits<uint8_t>::Read(stream);
    if (kind != (uint8_t)DumpObjectKind::Page)
        throw new DumpException();

    auto page = ReadCore(stream);
    page.RevisionIds = DumpTraits<std::set<std::uint32_t>>::Read(stream);

    return page;
}

void DumpPage::Write()
{
    if (wasLoaded && originalPage == page)
        return;

    DumpObject::Write();
}

void DumpPage::WriteDiff(DiffWriter &diffWriter)
{
    if (wasLoaded)
    {
        if (originalPage == page)
            diffWriter.StartExistingPage(page.PageId);
        else
            diffWriter.StartExistingPage(originalPage, page);
    }
    else
        diffWriter.StartNewPage(page);
}

void DumpPage::WriteInternal()
{
    WriteValue((uint8_t)DumpObjectKind::Page);
    WriteCore(*stream, page);
    WriteValue(page.RevisionIds);
}

void DumpPage::UpdateIndex(bool overwrite)
{
    auto dumpRef = dump.lock();

    if (overwrite)
        dumpRef->pageIdIndex->AddOrUpdate(page.PageId, savedOffset);
    else
        dumpRef->pageIdIndex->Add(page.PageId, savedOffset);
}

uint32_t DumpPage::NewLength()
{
    return ValueSize((uint8_t)DumpObjectKind::Page)
        + LengthCore(page)
        + ValueSize(page.RevisionIds);
}

DumpPage::DumpPage(weak_ptr<WritableDump> dump, uint32_t pageId)
    : DumpObject(dump), page()
{
    Load(pageId);
}

DumpPage::DumpPage(weak_ptr<WritableDump> dump, Offset offset)
    : DumpObject(dump), page()
{
    auto dumpRef = dump.lock();

    page = Read(dumpRef, offset);
    savedOffset = offset.value;
    savedLength = NewLength();
}

Page DumpPage::ReadCore(std::istream &stream)
{
    Page page;

    page.PageId = DumpTraits<std::uint32_t>::Read(stream);
    page.Namespace = DumpTraits<std::int16_t>::Read(stream);
    page.Title = DumpTraits<std::string>::Read(stream);
    page.RedirectTarget = DumpTraits<std::string>::Read(stream);

    return page;
}

void DumpPage::WriteCore(std::ostream &stream, const Page &page)
{
    WriteValue(stream, page.PageId);
    WriteValue(stream, page.Namespace);
    WriteValue(stream, page.Title);
    WriteValue(stream, page.RedirectTarget);
}

std::uint32_t DumpPage::LengthCore(const Page &page)
{
    return ValueSize(page.PageId)
        + ValueSize(page.Namespace) + ValueSize(page.Title) + ValueSize(page.RedirectTarget);
}