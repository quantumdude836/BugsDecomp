
// Private declarations for pad module.

#pragma once


// used for GetKeyName
typedef struct KEY_INFO
{
    char name[32];
    int scancode;
} KEY_INFO;


// keyboard reader function pointer
typedef void (*KBD_READ_FN)(void);


/// <summary>
/// Callback for DirectInput device enumeration. Used to find a joystick device.
/// </summary>
/// <param name="lpddi">Device instance</param>
/// <param name="pvRef">User-defined context</param>
/// <returns>Whether to continue or stop enumeration</returns>
EXTERN_C BOOL CALLBACK EnumDInputDevs(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef);
PATCH_CODE(0x402000, 0x402000, EnumDInputDevs);

/// <summary>
/// Reads input from the joystick.
/// </summary>
/// <param name="xAxis">Pointer to receive X-axis value</param>
/// <param name="yAxis">Pointer to receive Y-axis value</param>
/// <param name="buttons">Pointer to recieve bitmask of pressed buttons</param>
/// <returns>Whether input was successfully read</returns>
EXTERN_C BOOL ReadJoystick(long *xAxis, long *yAxis, int *buttons);
PATCH_CODE(0x402180, 0x402180, ReadJoystick);

/// <summary>
/// Initializes DirectInput resources for the game.
/// </summary>
/// <returns>Whether DirectInput was initialized successfully</returns>
EXTERN_C BOOL InitDInput(void);
PATCH_CODE(0x402610, 0x402610, InitDInput);

/// <summary>
/// Reads input from the keyboard via DirectInput. Uses ReadKbdWinMsg as a
/// fallback.
/// </summary>
EXTERN_C void ReadKbdDInput(void);
PATCH_CODE(0x402810, 0x402810, ReadKbdDInput);

/// <summary>
/// Reads input from the keyboard via Windows messages.
/// </summary>
EXTERN_C void ReadKbdWinMsg(void);
PATCH_CODE(0x4028a0, 0x4028a0, ReadKbdWinMsg);
