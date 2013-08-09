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
    Page page;

    auto &stream = *(dump->stream);
    stream.seekp(offset.value);

    auto kind = DumpTraits<uint8_t>::Read(stream);
    if (kind != (uint8_t)DumpObjectKind::Page)
        throw new DumpException();

    page.PageId = DumpTraits<uint32_t>::Read(stream);
    page.Namespace = DumpTraits<int16_t>::Read(stream);
    page.Title = DumpTraits<string>::Read(stream);
    page.RedirectTarget = DumpTraits<string>::Read(stream);
    page.RevisionIds = DumpTraits<vector<uint32_t>>::Read(stream);

    return page;
}

void DumpPage::Write()
{
    if (wasLoaded && originalPage == page)
        return;

    DumpObject::Write();
}

void DumpPage::WriteInternal()
{
    WriteValue((uint8_t)DumpObjectKind::Page);
    WriteValue(page.PageId);
    WriteValue(page.Namespace);
    // TODO: remove namespace from title for saving
    WriteValue(page.Title);
    WriteValue(page.RedirectTarget);
    WriteValue(page.RevisionIds);
}

void DumpPage::UpdateIndex(Offset offset, bool overwrite)
{
    auto dumpRef = dump.lock();

    if (overwrite)
        dumpRef->pageIdIndex->AddOrUpdate(page.PageId, offset);
    else
        dumpRef->pageIdIndex->Add(page.PageId, offset);
}

uint32_t DumpPage::NewLength()
{
    return ValueSize((uint8_t)DumpObjectKind::Page) + ValueSize(page.PageId)
        + ValueSize(page.Namespace) + ValueSize(page.Title) + ValueSize(page.RedirectTarget)
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
