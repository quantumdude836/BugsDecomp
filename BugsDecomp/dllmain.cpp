
#include "stdafx.h"
#include "GameVersion.h"
#include "CodePatch.h"


BOOL APIENTRY DllMain(
    HMODULE,
    DWORD ulReason,
    LPVOID
)
{
    switch (ulReason)
    {
    case DLL_PROCESS_ATTACH:
        // go ahead and try to detect game version; if it fails, don't allow DLL
        // to continue loading
        if (!detectVersion())
            return FALSE;

        // apply code patches; abort DLL load if any fail
        if (!CodePatch::applyAll())
            return FALSE;
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
