
// Config definitions (config.pc and bugs.ini).

#pragma once


// renderer options
enum RENDERER
{
    R_SOFT8, // software renderer, 8-bit color
    R_SOFT24, // software renderer, 24-bit color
    R_OPENGL, // hardware renderer (OpenGL)
};


// language options
enum LANG
{
    LANG_EN, // English
    LANG_FR, // French
    LANG_DE, // German
    LANG_ES, // Spanish
    LANG_IT, // Italian
    LANG_NL, // Dutch
};


// fog distance options
enum FOG_DIST
{
    FOG_NEAR,
    FOG_NORMAL,
    FOG_FAR,
};


// display type options
enum DISP_TYPE
{
    DISP_TYPE_UNK0,
    DISP_TYPE_UNK1,
    DISP_TYPE_UNK2,
};


// config.pc structure
struct CONFIG_PC
{
    int version;
    RENDERER renderer;
    LANG language;
    DISP_TYPE displayType;
    int width;
    int height;
    BOOL fullscreen;
    FOG_DIST fogDist;
    double field_20;
    double field_28;
    int soundVol; // percent
    int musicVol; // percent
    BOOL useJoystick;
    BYTE joyMap[10];
    BYTE kbdMap[16];
    BYTE field_56;
    BYTE field_57;
    double field_58;
    double field_60;
    double field_68;
    double field_70;
    BYTE field_78;
};

#define CONFIG_VERSION 0x10


// flags for config defaults
#define CFG_DEF_KBDMAP (1 << 0)
#define CFG_DEF_JOYMAP (1 << 1)
#define CFG_DEF_VIDEO  (1 << 2)
#define CFG_DEF_UNK3   (1 << 3)
#define CFG_DEF_SOUND  (1 << 4)
#define CFG_DEF_ALL    0xff


// string tables; each is an array of 5 string IDs
#define enStrTbl (reinterpret_cast<const int *>(0x45f2b8))
#define frStrTbl (reinterpret_cast<const int *>(0x45f2d0))
#define deStrTbl (reinterpret_cast<const int *>(0x45f2e8))
#define esStrTbl (reinterpret_cast<const int *>(0x45f300))
#define itStrTbl (reinterpret_cast<const int *>(0x45f318))
#define nlStrTbl (reinterpret_cast<const int *>(0x45f330))

// active string table
#define strTable (*reinterpret_cast<const int **>(0x45f344))


// alternate? configuration
#define altConfig (*reinterpret_cast<CONFIG_PC *>(0x9ca7a0))

// main, active configuration
#define config (*reinterpret_cast<CONFIG_PC *>(0x9ca820))


/// <summary>
/// Writes the current configuration to file.
/// </summary>
extern "C" void WriteConfig();
PATCH_CODE(0x409a40, 0x409a10, WriteConfig);

/// <summary>
/// Applies configuration defaults to a given config.
/// </summary>
/// <param name="flags">Which defaults to apply</param>
/// <param name="cfg">Config to modify</param>
extern "C" void SetConfigDefaults(BYTE flags, CONFIG_PC *cfg);
PATCH_CODE(0x409ac0, 0x409a90, SetConfigDefaults);

/// <summary>
/// Saves the current config to the alt config.
/// </summary>
extern "C" void SaveAltConfig();
PATCH_CODE(0x409ce0, 0x409c80, SaveAltConfig);

/// <summary>
/// Loads the alt config to the current config.
/// </summary>
extern "C" void LoadAltConfig();
PATCH_CODE(0x409d00, 0x409ca0, LoadAltConfig);

/// <summary>
/// Reads the current configuration from file, or loads defaults.
/// </summary>
extern "C" void ReadConfig();
PATCH_CODE(0x409d20, 0x409cc0, ReadConfig);
