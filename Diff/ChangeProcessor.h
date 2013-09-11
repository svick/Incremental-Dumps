#pragma once

#include "Changes/Change.h"
#include "Changes/SiteInfoChange.h"
#include "Changes/NewPageChange.h"
#include "Changes/PageChange.h"
#include "Changes/NewModelFormatChange.h"
#include "Changes/NewRevisionChange.h"
#include "Changes/RevisionChange.h"
#include "Changes/DeleteRevisionChange.h"
#include "Changes/FullDeletePageChange.h"
#include "Changes/PartialDeletePageChange.h"
#include "Changes/DiffTextGroup.h"
#include "../DumpObjects/DumpPage.h"

class ChangeProcessor
{
private:
    std::shared_ptr<WritableDump> dump;

    std::unique_ptr<DumpPage> currentPage;

    void WritePage();
public:
    ChangeProcessor(std::shared_ptr<WritableDump> dump);

    void Process(SiteInfoChange change);
    void Process(NewPageChange change);
    void Process(PageChange change);
    void Process(NewModelFormatChange change);
    void Process(NewRevisionChange change);
    void Process(RevisionChange change);
    void Process(DeleteRevisionChange change);
    void Process(FullDeletePageChange change);
    void Process(PartialDeletePageChange change);

    // has to be called after processing is complete
    void End();
};
