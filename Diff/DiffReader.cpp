#include <fstream>
#include "DiffReader.h"
#include "../DumpObjects/DumpObject.h"
#include "../DumpObjects/FileHeader.h"

const std::string Diff::MagicNumber = "MWDD";

DiffReader::DiffReader(std::string fileName, ChangeProcessor &changeProcessor)
    : stream(std::unique_ptr<std::istream>(new std::ifstream(fileName, std::ios::binary))),
        changeProcessor(changeProcessor)
{}

void DiffReader::Read()
{
    std::string magicNumber(Diff::MagicNumber.length(), '\0');
    stream->read(&magicNumber.at(0), Diff::MagicNumber.length());

    std::uint8_t fileFormatVersion, fileDataVersion;
    DumpObject::ReadValue(*stream, fileFormatVersion);
    DumpObject::ReadValue(*stream, fileDataVersion);

    if (magicNumber != Diff::MagicNumber
        || fileFormatVersion != FileHeader::FileFormatVersion
        || fileDataVersion != FileHeader::FileDataVersion)
        throw new DumpException();

    DumpKind dumpKind;
    DumpObject::ReadValue(*stream, dumpKind);

    ChangeKind siteInfoChangeKind;
    DumpObject::ReadValue(*stream, siteInfoChangeKind);
    if (siteInfoChangeKind != ChangeKind::SiteInfo)
        throw DumpException();

    auto siteInfo = SiteInfoChange::Read(*stream);
    changeProcessor.Process(siteInfo);

    while (true)
    {
        ChangeKind changeKind;
        DumpObject::ReadValue(*stream, changeKind);

        if (stream->eof())
            break;
    
        switch (changeKind)
        {
        case ChangeKind::NewPage:
            changeProcessor.Process(NewPageChange::Read(*stream));
            break;
        case ChangeKind::ChangePage:
            changeProcessor.Process(PageChange::Read(*stream));
            break;
        case ChangeKind::NewModelFormat:
            changeProcessor.Process(NewModelFormatChange::Read(*stream));
            break;
        case ChangeKind::NewRevision:
            changeProcessor.Process(NewRevisionChange::Read(*stream, IsPages(dumpKind)));
            break;
        case ChangeKind::ChangeRevision:
            changeProcessor.Process(RevisionChange::Read(*stream, IsPages(dumpKind)));
            break;
        case ChangeKind::DeleteRevision:
            changeProcessor.Process(DeleteRevisionChange::Read(*stream));
            break;
        case ChangeKind::DeletePage:
            changeProcessor.Process(DeletePageChange::Read(*stream));
            break;
        default:
            throw DumpException();
        }
    }

    changeProcessor.End();
}