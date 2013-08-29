#pragma once

#include "Changes/SiteInfoChange.h"
#include "Changes/NewPageChange.h"
#include "Changes/PageChange.h"
#include "Changes/NewModelFormatChange.h"
#include "Changes/NewRevisionChange.h"
#include "Changes/RevisionChange.h"
#include "Changes/DeleteRevisionChange.h"
#include "Changes/FullDeletePageChange.h"
#include "Changes/PartialDeletePageChange.h"

class ChangeVisitor
{
public:
    virtual void Visit(SiteInfoChange &change) = 0;
    virtual void Visit(NewPageChange &change) = 0;
    virtual void Visit(PageChange &change) = 0;
    virtual void Visit(NewModelFormatChange &change) = 0;
    virtual void Visit(NewRevisionChange &change) = 0;
    virtual void Visit(RevisionChange &change) = 0;
    virtual void Visit(DeleteRevisionChange &change) = 0;
    virtual void Visit(FullDeletePageChange &change) = 0;
    virtual void Visit(PartialDeletePageChange &change) = 0;
};