
#include "stdafx.h"
#include "dbg.h"


EXTERN_C void Dbg(const char *fmt, ...)
{
    char buf[1024];
    va_list ap;

    // format string
    va_start(ap, fmt);
    vsprintf_s(buf, fmt, ap);
    va_end(ap);

    // output to debugger
    OutputDebugStringA(buf);
}
