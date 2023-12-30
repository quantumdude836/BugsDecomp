
// Miscellaneous game definitions.

#pragma once


// game window handle
#define hWndGame (*reinterpret_cast<HWND *>(0x4b1cec))


/// <summary>
/// Writes formatted output to the console, if attached.
/// 
/// NOTE: this will likely not work, as the console handle it writes to is never
/// properly initialized.
/// </summary>
/// <param name="fmt">Format string</param>
extern "C" void ConPrintf(const char *fmt, ...);

/// <summary>
/// Shows a Retry/Cancel dialog with formatted text.
/// </summary>
/// <param name="fmt">Format string</param>
/// <returns>Selected dialog option</returns>
extern "C" int ShowRetryDialog(const char *fmt, ...);
