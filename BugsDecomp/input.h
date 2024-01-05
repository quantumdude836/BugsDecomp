
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
