
#include "stdafx.h"
#include "common.h"
#include "cfg.h"
#include "crt.h"
#include "fs.h"
#include "misc.h"
#include "rogl.h"
#include "scancodes.h"


void WriteConfig(void)
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

void SetConfigDefaults(BYTE flags, CONFIG_PC *cfg)
{
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
        if (TestGL())
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
        cfg->displayType = TestGL() ? DISP_TYPE_UNK1 : DISP_TYPE_UNK0;
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
        cfg->kbdMap[0] = SK_UP;
        cfg->kbdMap[1] = SK_DOWN;
        cfg->kbdMap[2] = SK_RIGHT;
        cfg->kbdMap[3] = SK_LEFT;
        cfg->kbdMap[4] = SK_Q;
        cfg->kbdMap[5] = SK_W;
        cfg->kbdMap[6] = SK_E;
        cfg->kbdMap[7] = SK_R;
        cfg->kbdMap[8] = SK_A;
        cfg->kbdMap[9] = SK_S;
        cfg->kbdMap[10] = SK_D;
        cfg->kbdMap[11] = SK_F;
        cfg->kbdMap[12] = SK_TAB;
        cfg->kbdMap[13] = SK_RETURN;
        cfg->kbdMap[14] = SK_TAB;
        cfg->kbdMap[15] = SK_RETURN;
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

void SaveAltConfig(void)
{
    altConfig = config;
}

void LoadAltConfig(void)
{
    config = altConfig;
}

void ReadConfig(void)
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
