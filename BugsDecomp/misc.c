
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

typedef struct TMD_HDR
{
    long flags;
    long num_objects;
} TMD_HDR;

typedef union TMD_REF
{
    long offset;
    void *ptr;
} TMD_REF;

typedef struct TMD_OBJECT
{
    TMD_REF vert_top;
    ULONG n_vert;
    TMD_REF normal_top;
    ULONG n_normal;
    TMD_REF primitive_top;
    ULONG n_primitive;
    long scale;
} TMD_OBJECT;

static inline float frand()
{
    return rand() / (float)RAND_MAX;
}

typedef struct TMD_VERT
{
    float x, y, z;
    int idx;
} TMD_VERT;

#define S 128.0f

void GsMapModelingData(void *p)
{
    TMD_HDR *hdr = p;
    TMD_OBJECT *obj = (TMD_OBJECT *)((DWORD)&hdr[1]);
    DWORD base = (DWORD)obj;
    for (int i = 0; i < hdr->num_objects; i++)
    {
        obj->vert_top.ptr = (void *)(base + obj->vert_top.offset);
        obj->normal_top.ptr = (void *)(base + obj->normal_top.offset);
        obj->primitive_top.ptr = (void *)(base + obj->primitive_top.offset);

        // lol
        for (int j = 0; j < obj->n_vert; j++)
        {
            TMD_VERT *v = (TMD_VERT *)obj->vert_top.ptr + j;
            v->x += (frand() * 2.0f - 1.0f) * S;
            v->y += (frand() * 2.0f - 1.0f) * S;
            v->z += (frand() * 2.0f - 1.0f) * S;
        }

        obj++;
    }
}
