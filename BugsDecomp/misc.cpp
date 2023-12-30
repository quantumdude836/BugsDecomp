
#include "stdafx.h"
#include "common.h"
#include "misc.h"


// console output handle; does not appear to be initialized by the game
#define hConsoleOutput (*reinterpret_cast<HANDLE *>(0x4b1cf4))


PATCH_CODE_1_0(0x4055d0, ConPrintf);
PATCH_CODE_1_6(0x4055d0, ConPrintf);
extern "C" void ConPrintf(const char *fmt, ...)
{
    DWORD nWritten;
    char buf[1024];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    WriteConsoleA(hConsoleOutput, buf, strlen(buf), &nWritten, nullptr);
}

PATCH_CODE_1_0(0x405640, ShowRetryDialog);
PATCH_CODE_1_6(0x405640, ShowRetryDialog);
extern "C" int ShowRetryDialog(const char *fmt, ...)
{
    char buf[1024];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);
    strcat(buf, BSTR(0x45f284, "\n"));

    return MessageBoxA(
        hWndGame,
        buf,
        BSTR(0x45f278, "Bugs Bunny"),
        MB_RETRYCANCEL
    );
}
