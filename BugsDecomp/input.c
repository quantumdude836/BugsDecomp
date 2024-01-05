
#include "stdafx.h"
#include "common.h"
#include "input.h"
#include "cfg.h"
#include "misc.h"


#define c_dfDIJoystick_bugs (*(const DIDATAFORMAT *)VER(0x44ed80, 0x44ebe0))

#define IID_IDirectInputDevice2A_bugs (*(const GUID *)0x45cc98)

// DirectInput instance
#define dinput (*(LPDIRECTINPUTA *)0x4b18c0)

// DI device for joystick
#define joystickDev (*(LPDIRECTINPUTDEVICE2A *)0x4b18c4)


BOOL CALLBACK EnumDInputDevs(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef)
{
    HRESULT hr;
    LPDIRECTINPUTDEVICEA dev;

    // try to create DI device for instance
    hr = IDirectInput_CreateDevice(dinput, &lpddi->guidInstance, &dev, NULL);
    if (FAILED(hr))
    {
        joystickDev = NULL;
        return DIENUM_CONTINUE;
    }

    // try to get IDirectInputDevice2A interface
    hr = IDirectInputDevice_QueryInterface(
        dev,
        &IID_IDirectInputDevice2A_bugs,
        (void **)&joystickDev
    );
    // go ahead and release the original device; if QueryInterface succeeded,
    // `joystickDev` will still have a reference; if it failed, this takes care
    // of cleanup
    IDirectInputDevice_Release(dev);
    if (FAILED(hr))
    {
        joystickDev = NULL;
        return DIENUM_CONTINUE;
    }

    config.useJoystick = TRUE;

    // setup standard joystick format
    IDirectInputDevice2_SetDataFormat(joystickDev, &c_dfDIJoystick_bugs);
    // link joystick to game window
    IDirectInputDevice2_SetCooperativeLevel(
        joystickDev,
        hWndGame,
        DISCL_BACKGROUND | DISCL_EXCLUSIVE
    );

    // set X/Y range
    DIPROPRANGE xRange = {
        {
            sizeof xRange,
            sizeof xRange.diph,
            0,
            DIPH_BYOFFSET,
        },
        -127,
        128,
    };
    IDirectInputDevice2_SetProperty(
        joystickDev,
        DIPROP_RANGE,
        (LPCDIPROPHEADER)&xRange
    );
    DIPROPRANGE yRange = {
        {
            sizeof yRange,
            sizeof yRange.diph,
            4,
            DIPH_BYOFFSET,
        },
        -127,
        128,
    };
    IDirectInputDevice2_SetProperty(
        joystickDev,
        DIPROP_RANGE,
        (LPCDIPROPHEADER)&yRange
    );

    // set Y/X deadzone
    DIPROPDWORD yDeadzone = {
        {
            sizeof yDeadzone,
            sizeof yDeadzone.diph,
            4,
            DIPH_BYOFFSET,
        },
        2000,
    };
    IDirectInputDevice2_SetProperty(
        joystickDev,
        DIPROP_DEADZONE,
        (LPCDIPROPHEADER)&yDeadzone
    );
    DIPROPDWORD xDeadzone = {
        {
            sizeof xDeadzone,
            sizeof xDeadzone.diph,
            0,
            DIPH_BYOFFSET,
        },
        2000,
    };
    IDirectInputDevice2_SetProperty(
        joystickDev,
        DIPROP_DEADZONE,
        (LPCDIPROPHEADER)&xDeadzone
    );

    return DIENUM_STOP;
}
