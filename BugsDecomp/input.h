
// Code for player input, including both DirectInput and Windows messages.

#pragma once


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
/// Initializes game input.
/// </summary>
EXTERN_C void InitInput(void);
PATCH_CODE(0x402fd0, 0x402fd0, InitInput);

/// <summary>
/// Gets the name of a joystick button. Returned pointer points to a shared
/// buffer, so the contents are invalidated after subsequent calls.
/// </summary>
/// <param name="button">Button code</param>
/// <returns>Pointer to button name, or NULL if invalid</returns>
EXTERN_C const char *GetButtonName(int button);
PATCH_CODE(0x403260, 0x403260, GetButtonName);
