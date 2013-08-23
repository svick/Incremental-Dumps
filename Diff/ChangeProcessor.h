#pragma once

#include "ChangeVisitor.h"
#include "Changes/Change.h"
#include "../DumpObjects/DumpPage.h"

class ChangeProcessor
{
private:
    std::shared_ptr<WritableDump> dump;

    std::unique_ptr<DumpPage> currentPage;

    std::unordered_set<std::uint32_t> visitedRevisionIds;

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
    void Process(DeletePageChange change);

    // has to be called after processing is complete
    void End();
};