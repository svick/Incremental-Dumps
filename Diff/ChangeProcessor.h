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

/**
 * Reads changes from diff dump and applies them to a dump.
 */
class ChangeProcessor
{
private:
    std::shared_ptr<WritableDump> dump;

    /**
     * The page that is currently being processed.
     */
    std::unique_ptr<DumpPage> currentPage;
    /**
     * ID of the text group from the last DiffTextGroup.
     *
     * When a change contains text ID, the text can be found in this group.
     */
    std::uint32_t currentTextGroupId;

    /**
     * Writes #currentPage to the dump and resets it.
     *
     * This method is called when it's clear that the current page won't be changed anymore
     * (e.g. when another page is started).
     */
    void WritePage();
public:
    /**
     * @param dump The dump to write to.
     */
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
    void Process(DiffTextGroup change);

    /**
     * Completes writing to the dump.
     * 
     * It's necessary to call this after processing is finished.
     */
    void Complete();
};