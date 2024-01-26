
#include "stdafx.h"
#include "common.h"
#include "misc.h"
#include "crt.h"
#include "enhancements.h"
#include "dbg.h"


// console output handle; does not appear to be initialized by the game
#define hConsoleOutput (*(HANDLE *)0x4b1cf4)


void ConPrintf(const char *fmt, ...)
{
    DWORD nWritten;
    char buf[1024];
    va_list ap;

    va_start(ap, fmt);
    vsprintf(buf, fmt, ap);
    va_end(ap);

    WriteConsoleA(hConsoleOutput, buf, strlen(buf), &nWritten, NULL);

#if ENH_DEBUG_CONSOLE
    Dbg("%s", buf);
#endif
}

int ShowRetryDialog(const char *fmt, ...)
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

int BugsRand(void)
{
    return rand_bugs();
}
