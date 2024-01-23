
// BZE level logic.

#pragma once


// Unit of level logic consistion of a condition and an action.
typedef struct LOGIC_ITEM
{
    // which condition to test
    BYTE condCode;
    // first condition argument
    BYTE condArg1;
    // second condition argument
    BYTE condArg2;
    // which action to run
    BYTE actionCode;
    // first action argument
    WORD actionArg1;
    // second action argument
    WORD actionArg2;
} LOGIC_ITEM;


/// <summary>
/// Tests the condition for a logic item.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to test condition for</param>
/// <returns>Condition result, or FALSE for invalid condition code</returns>
EXTERN_C BOOL TestLogicCond(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42a850, 0x42a7d0, TestLogicCond);

/// <summary>
/// Executes the action for a logic item.
/// </summary>
/// <param name="ent">Entity used for logic context</param>
/// <param name="item">Item to run action for</param>
EXTERN_C void RunLogicAction(void *ent, const LOGIC_ITEM *item);
PATCH_CODE(0x42c760, 0x42c6d0, RunLogicAction);
