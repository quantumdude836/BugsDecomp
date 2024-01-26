
// Miscellaneous game definitions.

#pragma once


// main game data (FIXME: relocate)
typedef struct GAME_DATA
{
    // currently loaded level ID
    int currentLevel;
    // checksum of game data; only used in PSX version
    int checksum;
    WORD field_8;
    WORD buttons_A;
    WORD buttons_C;
    WORD buttons_E;
    WORD buttons_10;
    short stickX;
    short stickY;
    WORD field_16;
    WORD field_18;
    WORD field_1A;
    WORD field_1C;
    WORD field_1E;
    WORD field_20;
    WORD field_22;
    WORD field_24;
    WORD field_26;
    WORD field_28;
    WORD field_2A;
    WORD field_2C;
    WORD field_2E;
    WORD field_30;
    WORD field_32;
    WORD field_34;
    WORD field_36;
    WORD field_38;
    WORD field_3A;
    WORD field_3C;
    WORD field_3E;
    // globals array for level logic
    BYTE logicGlobals[256];
} GAME_DATA;


// byte size of audio segments in the speech files; indexed by language, then by
// segment number (index 0 = segment 2?)
#define speechPartSizes (*(const size_t (*)[6][64])0x45c340)

// unknown; seems to take on one of four values:
//  0x00 - game exits
//  0x01 - game running normally
//  0x13 - recreates game window
//  0x14 - shows quit confirmation dialog
#define dword_45F2B0 (*(DWORD *)0x45f2b0)

// string tables; each is an array of 5 string IDs
#define enStrTbl (*(const int (*)[5])0x45f2b8)
#define frStrTbl (*(const int (*)[5])0x45f2d0)
#define deStrTbl (*(const int (*)[5])0x45f2e8)
#define esStrTbl (*(const int (*)[5])0x45f300)
#define itStrTbl (*(const int (*)[5])0x45f318)
#define nlStrTbl (*(const int (*)[5])0x45f330)

// active string table
#define strTable (*(const int **)0x45f344)

// process instance handle
#define hInstance (*(HINSTANCE *)0x4b1ce8)

// game window handle
#define hWndGame (*(HWND *)0x4b1cec)

// CPU speed, in MHz (FIXME: relocate)
#define cpuSpeed (*(int *)0x4b1de4)

// emulated PSX RAM for hardcoded addresses
#define psxRam (*(BYTE **)0x52fd00)

// currently playing speech track (FIXME: relocate)
#define speechTrack (*(struct TRACK *)0x5532a0)

// currently playing music track (FIXME: relocate)
#define musicTrack (*(struct TRACK *)0x553320)


// active game data; stored in emulated PSX RAM (FIXME: relocate)
#define gameData ((GAME_DATA *)(psxRam + 0x10000))


/// <summary>
/// Writes formatted output to the console, if attached.
/// 
/// NOTE: this will likely not work, as the console handle it writes to is never
/// properly initialized.
/// </summary>
/// <param name="fmt">Format string</param>
EXTERN_C void ConPrintf(const char *fmt, ...);
PATCH_CODE(0x4055d0, 0x4055d0, ConPrintf);

/// <summary>
/// Shows a Retry/Cancel dialog with formatted text.
/// </summary>
/// <param name="fmt">Format string</param>
/// <returns>Selected dialog option</returns>
EXTERN_C int ShowRetryDialog(const char *fmt, ...);
PATCH_CODE(0x405640, 0x405640, ShowRetryDialog);
