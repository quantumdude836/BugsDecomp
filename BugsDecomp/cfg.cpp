
#include "stdafx.h"
#include "common.h"
#include "cfg.h"
#include "crt.h"
#include "fs.h"
#include "misc.h"


PATCH_CODE(0x409a40, 0x409a10, WriteConfig);
extern "C" void WriteConfig()
{
    char path[MAX_PATH];

    strcpy(path, BSTR(0x4676b8, "..\\bin\\config.pc"));

    // not sure why the use of FsFOpen here; typically the datas root will point
    // to a read-only CD drive
    FILE *fp = FsFOpen(path, BSTR(0x4676b4, "wb+"));
    if (!fp)
        return;

    fwrite_bugs(&config, sizeof config, 1, fp);
    fclose_bugs(fp);
}

PATCH_CODE(0x409ac0, 0x409a90, SetConfigDefaults);
extern "C" void SetConfigDefaults(BYTE flags, CONFIG_PC *cfg)
{
    // FIXME: temp
    auto LoadGL = reinterpret_cast<int(*)()>(VER(0x40e0b0, 0x40e050));

    // always set version
    cfg->version = CONFIG_VERSION;

    // if any flags are specified, set the language and string table
    if (flags)
    {
        unsigned priLangId = GetSystemDefaultLangID() & 0x3ff;
        switch (priLangId)
        {
        case LANG_GERMAN:
            cfg->language = LANG_DE;
            strTable = deStrTbl;
            break;
        case LANG_ENGLISH:
            cfg->language = LANG_EN;
            strTable = enStrTbl;
            break;
        case LANG_SPANISH:
            cfg->language = LANG_ES;
            strTable = esStrTbl;
            break;
        case LANG_FRENCH:
            cfg->language = LANG_FR;
            strTable = frStrTbl;
            break;
        case LANG_ITALIAN:
            cfg->language = LANG_IT;
            strTable = itStrTbl;
            break;
        case LANG_DUTCH:
            cfg->language = LANG_NL;
            strTable = nlStrTbl;
            break;
        default:
            cfg->language = LANG_EN;
            strTable = enStrTbl;
            break;
        }
    }

    if (flags & CFG_DEF_VIDEO)
    {
        cfg->width = 512;
        cfg->height = 384;

        // try to initialize GL; use success to set renderer
        if (LoadGL())
            cfg->renderer = R_OPENGL;
        else
        {
            // if CPU is "fast" (aka above 300MHz), use 24-bit color; else 8-bit
            if (cpuSpeed >= 300)
                cfg->renderer = R_SOFT24;
            else
                cfg->renderer = R_SOFT8;
        }

        cfg->fogDist = FOG_FAR;
        cfg->field_C = LoadGL();
        cfg->fullscreen = TRUE;
        cfg->field_20 = 1.0;
    }

    if (flags & CFG_DEF_JOYMAP)
    {
        cfg->useJoystick = FALSE;
        cfg->joyMap[0] = 0;
        cfg->joyMap[1] = 1;
        cfg->joyMap[2] = 2;
        cfg->joyMap[3] = 3;
        cfg->joyMap[4] = 4;
        cfg->joyMap[5] = 5;
        cfg->joyMap[6] = 6;
        cfg->joyMap[7] = 7;
        cfg->joyMap[8] = 8;
        cfg->joyMap[9] = 9;
    }

    if (flags & CFG_DEF_KBDMAP)
    {
        cfg->kbdMap[0] = 0xc8; // up arrow
        cfg->kbdMap[1] = 0xd0; // down arrow
        cfg->kbdMap[2] = 0xcd; // right arrow
        cfg->kbdMap[3] = 0xcb; // left arrow
        cfg->kbdMap[4] = 0x10; // Q
        cfg->kbdMap[5] = 0x11; // W
        cfg->kbdMap[6] = 0x12; // E
        cfg->kbdMap[7] = 0x13; // R
        cfg->kbdMap[8] = 0x1e; // A
        cfg->kbdMap[9] = 0x1f; // S
        cfg->kbdMap[10] = 0x20; // D
        cfg->kbdMap[11] = 0x21; // F
        cfg->kbdMap[12] = 0x0f; // tab?
        cfg->kbdMap[13] = 0x1c; // enter?
        cfg->kbdMap[14] = 0x0f; // tab?
        cfg->kbdMap[15] = 0x1c; // enter?
    }

    if (flags & CFG_DEF_SOUND)
    {
        cfg->soundVol = 100;
        cfg->musicVol = 100;
    }

    if (flags & CFG_DEF_UNK3)
    {
        cfg->field_28 = 1.0;
        cfg->field_58 = 0.0;
        cfg->field_60 = 1.0;
        cfg->field_68 = 1.0;
        cfg->field_70 = 1.0;
        cfg->field_56 = 0x10;
        cfg->field_57 = 0x01;
        cfg->field_78 = 0x00;
    }
}

PATCH_CODE(0x409ce0, 0x409c80, SaveAltConfig);
extern "C" void SaveAltConfig()
{
    altConfig = config;
}

PATCH_CODE(0x409d00, 0x409ca0, LoadAltConfig);
extern "C" void LoadAltConfig()
{
    config = altConfig;
}

PATCH_CODE(0x409d20, 0x409cc0, ReadConfig);
extern "C" void ReadConfig()
{
    char path[MAX_PATH];

    // try loading config from file
    strcpy(path, BSTR(0x4676b8, "..\\bin\\config.pc"));
    FILE *fp = fopen_bugs(path, BSTR(0x45f3b4, "rb"));
    if (fp)
    {
        // make sure it's the right size
        fseek_bugs(fp, 0, SEEK_END);
        long size = ftell_bugs(fp);
        fseek_bugs(fp, 0, SEEK_SET);
        if (size == sizeof(CONFIG_PC))
        {
            // go ahead and load it
            fread_bugs(&config, sizeof config, 1, fp);
            fclose_bugs(fp);
            // version check
            if (config.version == CONFIG_VERSION)
                return;
            // bad version; overwrite config with defaults
        }
        else
        {
            // wrong size; ignore it and load defaults
            fclose_bugs(fp);
        }
    }

    // load all defaults into config
    SetConfigDefaults(CFG_DEF_ALL, &config);
    // do the same for the alt config, but make it windowed
    SetConfigDefaults(CFG_DEF_ALL, &altConfig);
    altConfig.fullscreen = FALSE;

    // write the default config to file
    WriteConfig();
}
