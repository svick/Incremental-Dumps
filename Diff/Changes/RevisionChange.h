#pragma once

#include "Change.h"
#include "../../Objects/Revision.h"

enum class RevisionChangeFlags : std::uint8_t
{
    NoChanges   = 0x00,

    Flags       = 0x01,
    ParentId    = 0x02,
    DateTime    = 0x04,
    Contributor = 0x08,
    Comment     = 0x10,
    Text        = 0x20,
    ModelFormat = 0x40
};

RevisionChangeFlags operator &(RevisionChangeFlags first, RevisionChangeFlags second);
RevisionChangeFlags operator |(RevisionChangeFlags first, RevisionChangeFlags second);
RevisionChangeFlags operator |=(RevisionChangeFlags &first, RevisionChangeFlags second);
bool HasFlag(RevisionChangeFlags value, RevisionChangeFlags flag);

class RevisionChange : public Change
{
private:
    bool withText;

    RevisionChange(bool withText);
public:
    RevisionChangeFlags flags;

    Revision revisionChanges;
    std::uint8_t newRevisionModelFormatId;

    RevisionChange(
        const Revision &oldRevision, Revision &newRevision,
        std::uint8_t newRevisionModelFormatId, bool withText);

    static RevisionChange Read(std::istream &stream, bool withText);
    virtual void WriteInternal() override;
    virtual std::uint32_t NewLength() override;

    virtual void Accept(ChangeVisitor &visitor) override;
};