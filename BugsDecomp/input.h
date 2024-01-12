
// Code for player input, including both DirectInput and Windows messages.

#pragma once


// mask of extra input keys that aren't mapped to PSX buttons; bit 0 = return,
// bit 1 = escape, bit 2 = space
#define extraInputKeys (*(WORD *)0x4b18d0)


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
/// Gets the name of a keyboard key.
/// </summary>
/// <param name="scancode">Keyboard key scancode</param>
/// <returns>Pointer to button name, or NULL if invalid</returns>
EXTERN_C const char *GetKeyName(int scancode);
PATCH_CODE(0x402250, 0x402250, GetKeyName);

/// <summary>
/// Reads keyboard input and updates the pad 1 input report using an alternative
/// key mapping. Changes the input report report type to 16-button.
/// </summary>
EXTERN_C void MapAltKeyboardInput(void);
PATCH_CODE(0x402290, 0x402290, MapAltKeyboardInput);

/// <summary>
/// Reads keyboard input and updates the pad 1 input report. May change the
/// input report type to 16-button. Also sets extra input keys.
/// </summary>
EXTERN_C void MapKeyboardInput(void);
PATCH_CODE(0x4023e0, 0x4023e0, MapKeyboardInput);

/// <summary>
/// Initializes DirectInput resources for the game.
/// </summary>
/// <returns>Whether DirectInput was initialized successfully</returns>
EXTERN_C BOOL InitDInput(void);
PATCH_CODE(0x402610, 0x402610, InitDInput);

/// <summary>
/// Finalizes DirectInput resources for the game.
/// </summary>
EXTERN_C void FiniDInput(void);
PATCH_CODE(0x402790, 0x402790, FiniDInput);

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

/// <summary>
/// Initializes game input.
/// </summary>
EXTERN_C void InitInput(void);
PATCH_CODE(0x402fd0, 0x402fd0, InitInput);

/// <summary>
/// Sets buffers to be used for emulated PSX pad reports.
/// </summary>
/// <param name="pad1">Destination for pad 1</param>
/// <param name="pad2">Destination for pad 2; unused by PC</param>
EXTERN_C void PadInitDirect(BYTE *pad1, BYTE *pad2);
PATCH_CODE(0x402ff0, 0x402ff0, PadInitDirect);

/// <summary>
/// Reads joystick input and maps it to an analog joystick PSX input report for
/// pad 1. If no joystick is attached, sets the pad 1 input report to a default
/// 16-button state.
/// </summary>
EXTERN_C void MapJoystickInput(void);
PATCH_CODE(0x403010, 0x403010, MapJoystickInput);

/// <summary>
/// Reads joystick input and checks if a button is being pressed.
/// </summary>
/// <returns>
/// The index of a pressed button, 0xFE if nothing pressed, or 0xFF on failure
/// </returns>
EXTERN_C int GetPressedButton(void);
PATCH_CODE(0x403210, 0x403210, GetPressedButton);

/// <summary>
/// Gets the name of a joystick button. Returned pointer points to a shared
/// buffer, so the contents are invalidated after subsequent calls.
/// </summary>
/// <param name="button">Button code</param>
/// <returns>Pointer to button name, or NULL if invalid</returns>
EXTERN_C const char *GetButtonName(int button);
PATCH_CODE(0x403260, 0x403260, GetButtonName);
