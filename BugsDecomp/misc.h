
// Miscellaneous game definitions.

#pragma once


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

// currently playing speech track (FIXME: relocate)
#define speechTrack (*(struct TRACK *)0x5532a0)

// currently playing music track (FIXME: relocate)
#define musicTrack (*(struct TRACK *)0x553320)


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
