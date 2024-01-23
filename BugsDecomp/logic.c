
#include "stdafx.h"
#include "common.h"
#include "logic.h"
#include "logic_prv.h"


// condition handler table
#define condTable (*(LOGIC_COND_FN (*)[99])0x4ac550)

// action handler table
#define actionTable (*(LOGIC_ACTION_FN (*)[129])0x4ac6e0)


BOOL TestLogicCond(void *ent, const LOGIC_ITEM *item)
{
    if (item->condCode < ARRAYSIZE(condTable))
        return condTable[item->condCode](ent, item);
    return FALSE;
}

void RunLogicAction(void *ent, const LOGIC_ITEM *item)
{
    if (item->actionCode < ARRAYSIZE(actionTable))
        actionTable[item->actionCode](ent, item);
}
