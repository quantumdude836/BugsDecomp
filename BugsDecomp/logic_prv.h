
// Private definitions for logic module.

#pragma once


// handler type for logic conditions
typedef BOOL (*LOGIC_COND_FN)(void *, const LOGIC_ITEM *);

// handler type for logic actions
typedef void (*LOGIC_ACTION_FN)(void *, const LOGIC_ITEM *);
