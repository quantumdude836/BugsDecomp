
#include "stdafx.h"
#include "common.h"
#include "logic.h"
#include "logic_prv.h"
#include "misc.h"
#include "pad.h"
#include "enhancements.h"
#include "dbg.h"


// number of defined condition codes
#define LOGIC_COND_COUNT 99

// number of defined action codes
#define LOGIC_ACTION_COUNT 129


// condition handler table
#define condTable (*(LOGIC_COND_FN (*)[LOGIC_COND_COUNT])0x4ac550)

// action handler table
#define actionTable (*(LOGIC_ACTION_FN (*)[LOGIC_ACTION_COUNT])0x4ac6e0)

// 256-byte scratch space
#define logicScratch (*(BYTE (*)[256])0x4b2260)


// "shortcuts" for handlers
#define SCRATCH (logicScratch)
#define GLOBALS (gameData->logicGlobals)


#if ENH_DEBUG_LOGIC

// helpers for converting condition/action codes to strings

static const char *const condNameTable[LOGIC_COND_COUNT] = {
    [0x00] = "true",
    [0x01] = "ciseq",
    [0x02] = "cigeq",
    [0x03] = "cisltu",
    [0x04] = "cigltu",
    [0x05] = "cisleu",
    [0x06] = "cigleu",
    [0x07] = "cisgtu",
    [0x08] = "ciggtu",
    [0x09] = "cisgeu",
    [0x0a] = "ciggeu",
    [0x0f] = "csseq",
    [0x10] = "cggeq",
    [0x1e] = "tismnz",
    [0x1f] = "tigmnz",
    [0x22] = "tismz",
    [0x23] = "tigmz",
    [0x25] = "cisne",
    [0x26] = "cigne",
    [0x27] = "tisaz",
    [0x28] = "tigaz",
    [0x2e] = "tisxz",
    [0x2f] = "tigxz",
    [0x31] = "tbdact",
    [0x33] = "tpnmv",
    [0x35] = "tpmov",
    [0x3d] = "cseq",
    [0x3e] = "cgeq",
    [0x3f] = "csne",
    [0x40] = "cgne",
    [0x41] = "csltu",
    [0x42] = "cgltu",
    [0x43] = "csgtu",
    [0x44] = "cggtu",
    [0x46] = "cislt",
    [0x47] = "cisgt",
    [0x48] = "cslt",
    [0x49] = "csgt",
    [0x51] = "tbuact",
    [0x58] = "ciglt",
    [0x59] = "ciggt",
    [0x60] = "tkdesc",
};

static inline const char *CondName(BYTE code)
{
    static char buf[8];
    const char *name = NULL;

    // if code is valid, see if there's a known name
    if (code < LOGIC_COND_COUNT)
        name = condNameTable[code];

    // generate default name if needed
    if (!name)
    {
        sprintf_s(buf, sizeof buf, "cnd_%02x", code);
        name = buf;
    }

    return name;
}

static const char *const actionNameTable[LOGIC_ACTION_COUNT] = {
    [0x00] = "nop",
    [0x03] = "incs",
    [0x04] = "incg",
    [0x05] = "decs",
    [0x06] = "decg",
    [0x07] = "clrs",
    [0x08] = "clrg",
    [0x09] = "negs",
    [0x0a] = "negg",
    [0x0b] = "orsi",
    [0x0c] = "orgi",
    [0x0d] = "andsi",
    [0x0e] = "andgi",
    [0x0f] = "rnds",
    [0x10] = "rndg",
    [0x12] = "movsi",
    [0x13] = "movgi",
    [0x14] = "movss",
    [0x15] = "movgg",
    [0x16] = "incss",
    [0x17] = "incgg",
    [0x1b] = "movgs",
    [0x1c] = "movsg",
    [0x24] = "addsi",
    [0x25] = "addgi",
    [0x27] = "subsi",
    [0x28] = "subgi",
    [0x30] = "gvcrt",
};

static inline const char *ActionName(BYTE code)
{
    static char buf[8];
    const char *name = NULL;

    // if code is valid, see if there's a known name
    if (code < LOGIC_ACTION_COUNT)
        name = actionNameTable[code];

    // generate default name if needed
    if (!name)
    {
        sprintf_s(buf, sizeof buf, "act_%02x", code);
        name = buf;
    }

    return name;
}

#endif


// helper for dealing with 2-byte golden carrot count

static inline void AddGlobal(WORD offset, WORD adj)
{
    // targeting the total GCs?
    if (offset == 0x07)
    {
        // read bytes and merge
        WORD gcTotal = (GLOBALS[0xfc] << 8) | GLOBALS[0x07];
        // adjust value
        gcTotal += adj;
        // split bytes and write
        GLOBALS[0x07] = gcTotal & 0xff;
        GLOBALS[0xfc] = gcTotal >> 8;
    }
    else
    {
        // just adjust the byte
        GLOBALS[offset] += adj;
    }
}


BOOL TestLogicCond(void *ent, const LOGIC_ITEM *item)
{
    BOOL res = FALSE;
    if (item->condCode < LOGIC_COND_COUNT)
        res = condTable[item->condCode](ent, item);

#if ENH_DEBUG_LOGIC
    const char *name = CondName(item->condCode);
    Dbg(
        "CND(0x%08x) %-7s 0x%02x, 0x%02x -> %c\n",
        (DWORD)ent,
        name,
        item->condArg1,
        item->condArg2,
        res ? '1' : '0'
    );
#endif

    return res;
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
#if ENH_DEBUG_LOGIC
    const char *name = ActionName(item->actionCode);
    Dbg(
        "ACT(0x%08x) %-7s 0x%04x, 0x%04x\n",
        (DWORD)ent,
        name,
        item->actionArg1,
        item->actionArg2
    );
#endif

    if (item->actionCode < LOGIC_ACTION_COUNT)
        actionTable[item->actionCode](ent, item);
}

void ActionNop(void *ent, const LOGIC_ITEM *item)
{
}

void ActionIncs(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2]++;
}

void ActionIncg(void *ent, const LOGIC_ITEM *item)
{
    AddGlobal(item->actionArg2, 1);
}

void ActionMovsi(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2] = (BYTE)item->actionArg1;
}

void ActionMovgi(void *ent, const LOGIC_ITEM *item)
{
    GLOBALS[item->actionArg2] = (BYTE)item->actionArg1;
}

void ActionDecs(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2]--;
}

void ActionDecg(void *ent, const LOGIC_ITEM *item)
{
    AddGlobal(item->actionArg2, -1);
}

void ActionClrs(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2] = 0;
}

void ActionClrg(void *ent, const LOGIC_ITEM *item)
{
    GLOBALS[item->actionArg2] = 0;
}

void ActionNegs(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2] *= -1;
}

void ActionNegg(void *ent, const LOGIC_ITEM *item)
{
    GLOBALS[item->actionArg2] *= -1;
}

void ActionOrsi(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2] |= item->actionArg1;
}

void ActionOrgi(void *ent, const LOGIC_ITEM *item)
{
    GLOBALS[item->actionArg2] |= item->actionArg1;
}

void ActionAndsi(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2] &= item->actionArg1;
}

void ActionAndgi(void *ent, const LOGIC_ITEM *item)
{
    GLOBALS[item->actionArg2] &= item->actionArg1;
}

void ActionMovss(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2] = SCRATCH[item->actionArg1];
}

void ActionMovgg(void *ent, const LOGIC_ITEM *item)
{
    GLOBALS[item->actionArg2] = GLOBALS[item->actionArg1];
}

void ActionMovgs(void *ent, const LOGIC_ITEM *item)
{
    GLOBALS[item->actionArg2] = SCRATCH[item->actionArg1];
}

void ActionMovsg(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2] = GLOBALS[item->actionArg1];
}

void ActionIncss(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2]++;
    SCRATCH[item->actionArg1]++;
}

void ActionIncgg(void *ent, const LOGIC_ITEM *item)
{
    AddGlobal(item->actionArg2, 1);
    AddGlobal(item->actionArg1, 1);
}

void ActionRnds(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2] = BugsRand() % item->actionArg1;
}

void ActionRndg(void *ent, const LOGIC_ITEM *item)
{
    GLOBALS[item->actionArg2] = BugsRand() % item->actionArg1;
}

void ActionAddsi(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2] += item->actionArg1;
}

void ActionAddgi(void *ent, const LOGIC_ITEM *item)
{
    AddGlobal(item->actionArg2, item->actionArg1);
}

void ActionSubsi(void *ent, const LOGIC_ITEM *item)
{
    SCRATCH[item->actionArg2] -= item->actionArg1;
}

void ActionSubgi(void *ent, const LOGIC_ITEM *item)
{
    AddGlobal(item->actionArg2, -item->actionArg1);
}

void ActionGvcrt(void *ent, const LOGIC_ITEM *item)
{
    // is player at full health?
    BYTE curHealth = GLOBALS[0x01];
    BYTE maxHealth = GLOBALS[0x02];
    if (curHealth != maxHealth)
    {
        // no; increment health
        GLOBALS[0x01]++;
    }
    else
    {
        // yes; increment other global (assume it's NOT total GC count)
        GLOBALS[item->actionArg2]++;
    }
}
