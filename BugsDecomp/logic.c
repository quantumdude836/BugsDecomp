
#include "stdafx.h"
#include "common.h"
#include "logic.h"
#include "logic_prv.h"
#include "misc.h"
#include "pad.h"


// condition handler table
#define condTable (*(LOGIC_COND_FN (*)[99])0x4ac550)

// action handler table
#define actionTable (*(LOGIC_ACTION_FN (*)[129])0x4ac6e0)

// 256-byte scratch space
#define logicScratch (*(BYTE (*)[256])0x4b2260)


// "shortcuts" for handlers
#define SCRATCH (logicScratch)
#define GLOBALS (gameData->logicGlobals)


BOOL TestLogicCond(void *ent, const LOGIC_ITEM *item)
{
    if (item->condCode < ARRAYSIZE(condTable))
        return condTable[item->condCode](ent, item);
    return FALSE;
}

BOOL CondTrue(void *ent, const LOGIC_ITEM *item)
{
    return TRUE;
}

BOOL CondCiseq(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 == SCRATCH[item->condArg2];
}

BOOL CondCseq(void *ent, const LOGIC_ITEM *item)
{
    return SCRATCH[item->condArg1] == SCRATCH[item->condArg2];
}

BOOL CondCisne(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 != SCRATCH[item->condArg2];
}

BOOL CondCsne(void *ent, const LOGIC_ITEM *item)
{
    return SCRATCH[item->condArg1] != SCRATCH[item->condArg2];
}

BOOL CondCsseq(void *ent, const LOGIC_ITEM *item)
{
    return SCRATCH[item->condArg1] == SCRATCH[item->condArg2];
}

BOOL CondCisltu(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 < SCRATCH[item->condArg2];
}

BOOL CondCislt(void *ent, const LOGIC_ITEM *item)
{
    return (INT8)item->condArg1 < (INT8)SCRATCH[item->condArg2];
}

BOOL CondCsltu(void *ent, const LOGIC_ITEM *item)
{
    return SCRATCH[item->condArg1] < SCRATCH[item->condArg2];
}

BOOL CondCslt(void *ent, const LOGIC_ITEM *item)
{
    return (INT8)SCRATCH[item->condArg1] < (INT8)SCRATCH[item->condArg2];
}

BOOL CondCisleu(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 <= SCRATCH[item->condArg2];
}

BOOL CondCisgtu(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 > SCRATCH[item->condArg2];
}

BOOL CondCisgt(void *ent, const LOGIC_ITEM *item)
{
    return (INT8)item->condArg1 > (INT8)SCRATCH[item->condArg2];
}

BOOL CondCsgtu(void *ent, const LOGIC_ITEM *item)
{
    return SCRATCH[item->condArg1] > SCRATCH[item->condArg2];
}

BOOL CondCsgt(void *ent, const LOGIC_ITEM *item)
{
    return (INT8)SCRATCH[item->condArg1] > (INT8)SCRATCH[item->condArg2];
}

BOOL CondCisgeu(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 >= SCRATCH[item->condArg2];
}

BOOL CondTismnz(void *ent, const LOGIC_ITEM *item)
{
    return (item->condArg1 & SCRATCH[item->condArg2]) != 0;
}

BOOL CondTisaz(void *ent, const LOGIC_ITEM *item)
{
    return (item->condArg1 & SCRATCH[item->condArg2]) == 0;
}

BOOL CondTismz(void *ent, const LOGIC_ITEM *item)
{
    return (item->condArg1 & SCRATCH[item->condArg2]) == 0;
}

BOOL CondCigeq(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 == GLOBALS[item->condArg2];
}

BOOL CondCgeq(void *ent, const LOGIC_ITEM *item)
{
    return GLOBALS[item->condArg1] == GLOBALS[item->condArg2];
}

BOOL CondCigne(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 != GLOBALS[item->condArg2];
}

BOOL CondCgne(void *ent, const LOGIC_ITEM *item)
{
    return GLOBALS[item->condArg1] != GLOBALS[item->condArg2];
}

BOOL CondCggeq(void *ent, const LOGIC_ITEM *item)
{
    return GLOBALS[item->condArg1] == GLOBALS[item->condArg2];
}

BOOL CondCigltu(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 < GLOBALS[item->condArg2];
}

BOOL CondCiglt(void *ent, const LOGIC_ITEM *item)
{
    return (INT8)item->condArg1 < (INT8)GLOBALS[item->condArg2];
}

BOOL CondCgltu(void *ent, const LOGIC_ITEM *item)
{
    return GLOBALS[item->condArg1] < GLOBALS[item->condArg2];
}

BOOL CondCigleu(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 <= GLOBALS[item->condArg2];
}

BOOL CondCiggtu(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 > GLOBALS[item->condArg2];
}

BOOL CondCiggt(void *ent, const LOGIC_ITEM *item)
{
    return (INT8)item->condArg1 > (INT8)GLOBALS[item->condArg2];
}

BOOL CondCggtu(void *ent, const LOGIC_ITEM *item)
{
    return GLOBALS[item->condArg1] > GLOBALS[item->condArg2];
}

BOOL CondCiggeu(void *ent, const LOGIC_ITEM *item)
{
    return item->condArg1 >= GLOBALS[item->condArg2];
}

BOOL CondTigmnz(void *ent, const LOGIC_ITEM *item)
{
    return (item->condArg1 & GLOBALS[item->condArg2]) != 0;
}

BOOL CondTigaz(void *ent, const LOGIC_ITEM *item)
{
    return (item->condArg1 & GLOBALS[item->condArg2]) == 0;
}

BOOL CondTigmz(void *ent, const LOGIC_ITEM *item)
{
    return (item->condArg1 & GLOBALS[item->condArg2]) == 0;
}

BOOL CondTisxz(void *ent, const LOGIC_ITEM *item)
{
    return (item->condArg1 ^ SCRATCH[item->condArg2]) == 0;
}

BOOL CondTigxz(void *ent, const LOGIC_ITEM *item)
{
    return (item->condArg1 ^ GLOBALS[item->condArg2]) == 0;
}

BOOL CondTbdact(void *ent, const LOGIC_ITEM *item)
{
    return (gameData->buttons_A >> 3) & 1;
}

BOOL CondTbuact(void *ent, const LOGIC_ITEM *item)
{
    return (~gameData->buttons_A >> 3) & 1;
}

BOOL CondTkdesc(void *ent, const LOGIC_ITEM *item)
{
    return (extraInputKeys & (1 << 1)) >> 1;
}

void RunLogicAction(void *ent, const LOGIC_ITEM *item)
{
    if (item->actionCode < ARRAYSIZE(actionTable))
        actionTable[item->actionCode](ent, item);
}
