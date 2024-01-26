
// Code for PSX gamepad input emulation via DirectInput and Windows messages.

#pragma once


#define PadStateStable 6


// mask of extra input keys that aren't mapped to PSX buttons; bit 0 = return,
// bit 1 = escape, bit 2 = space
#define extraInputKeys (*(short *)0x4b18d0)


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
/// Finalizes DirectInput resources for the game.
/// </summary>
EXTERN_C void FiniDInput(void);
PATCH_CODE(0x402790, 0x402790, FiniDInput);

/// <summary>
/// Starts reading input from emulated PSX gamepad.
/// </summary>
EXTERN_C void PadStartCom(void);
PATCH_CODE(0x402fd0, 0x402fd0, PadStartCom);

/// <summary>
/// Gets emulated PSX gamepad state.
/// </summary>
/// <param name="port">Port number to check</param>
/// <returns>Pad state; always returns PadStateStable</returns>
EXTERN_C int PadGetState(int port);
PATCH_CODE(0x402fe0, 0x402fe0, PadGetState);

/// <summary>
/// Gets controller mode information.
///
/// Emulated gamepad does not support vibration, so this always returns 0.
///
/// This function does not separately exist in the game, but is merged with
/// other functions which just return 0.
/// </summary>
/// <param name="port">Controller port number</param>
/// <param name="term">Item to check</param>
/// <param name="offs">Controller mode ID table offset</param>
/// <returns>Always returns 0 for no vibration support</returns>
EXTERN_C int PadInfoMode(int port, int term, int offs);

/// <summary>
/// Sets controller actuator parameters.
///
/// Requests are always rejected for PC.
///
/// This function does not separately exist in the game, but is merged with
/// other functions which just return 0.
/// </summary>
/// <param name="port">Controller port number</param>
/// <param name="data">Pointer to actuator data</param>
/// <returns>Always returns 0 for failure</returns>
EXTERN_C int PadSetActAlign(int port, BYTE *data);

/// <summary>
/// Sets controller mode selector.
///
/// Requests are always rejected for PC.
///
/// This function does not separately exist in the game, but is merged with
/// other functions which just return 0.
/// </summary>
/// <param name="port">Controller port number</param>
/// <param name="offs">Controller mode ID table offset</param>
/// <param name="lock">Lock bits</param>
/// <returns>Always returns 0 for failure</returns>
EXTERN_C int PadSetMainMode(int port, int offs, int lock);

/// <summary>
/// Sets transmit buffer for controller.
///
/// This does nothing for PC.
///
/// This function does not separately exist in the game, but is merged with
/// all other functions which do nothing.
/// </summary>
/// <param name="port">Controller port number</param>
/// <param name="data">Pointer to transmit buffer</param>
/// <param name="len">Length, in bytes, of transmit buffer</param>
EXTERN_C void PadSetAct(int port, BYTE *data, int len);

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
