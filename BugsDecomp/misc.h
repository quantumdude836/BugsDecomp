
// Miscellaneous game definitions.

#pragma once


// game window handle
#define hWndGame (*(HWND *)0x4b1cec)

// CPU speed, in MHz (FIXME: relocate)
#define cpuSpeed (*(int *)0x4b1de4)


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
