
#include "stdafx.h"
#include "common.h"
#include "input.h"
#include "cfg.h"
#include "misc.h"


// DirectInput data formats
#define c_dfDIJoystick_bugs (*(const DIDATAFORMAT *)VER(0x44ed80, 0x44ebe0))
#define c_dfDIKeyboard_bugs (*(const DIDATAFORMAT *)VER(0x44fda0, 0x44fc00))

// DirectInput IDs
#define IID_IDirectInputDevice2A_bugs (*(const IID *)0x45cc98)
#define GUID_SysKeyboard_bugs (*(const GUID *)0x45cca8)
#define CLSID_DirectInput_bugs (*(const CLSID *)0x45ccb8)
#define IID_IDirectInputA_bugs (*(const IID *)0x45ccc8)

// DirectInput instance
#define dinput (*(LPDIRECTINPUTA *)0x4b18c0)

// DI device for joystick
#define joystickDev (*(LPDIRECTINPUTDEVICE2A *)0x4b18c4)

// DI device for keyboard
#define keyboardDev (*(LPDIRECTINPUTDEVICEA *)0x4b18c8)

// whether keyboard DI device is currently acquired
#define kbdAcquired (*(BOOL *)0x9ca8a0)

// whether keyboard input uses DirectInput (or Windows messages)
#define kbdUsesDInput (*(BOOL *)0x9ca9c4)


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

BOOL ReadJoystick(long *xAxis, long *yAxis, int *buttons)
{
    HRESULT hr;
    DIJOYSTATE joyState;

    *xAxis = 0;
    *yAxis = 0;
    *buttons = 0;

    // no joystick -> no input
    if (!joystickDev)
        return FALSE;

    // poll the device, (re)acquiring if needed
    hr = IDirectInputDevice2_Poll(joystickDev);
    if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
    {
        IDirectInputDevice2_Acquire(joystickDev);
        hr = IDirectInputDevice2_Poll(joystickDev);
    }
    // if poll still failed, give up
    if (FAILED(hr))
        return FALSE;

    // read state
    hr = IDirectInputDevice2_GetDeviceState(
        joystickDev,
        sizeof joyState,
        &joyState
    );
    if (FAILED(hr))
        return FALSE;

    // copy (main) X/Y values
    *xAxis = joyState.lX;
    *yAxis = joyState.lY;

    // collect buttons into bitmask
    *buttons = 0;
    for (int button = 0; button < 32; button++)
    {
        if (joyState.rgbButtons[button])
            *buttons |= 1 << button;
    }

    return TRUE;
}

BOOL InitDInput(void)
{
    HRESULT hr;
    GUID kbdGuid;

    // clean up first to prevent resource leaks
    FiniDInput();

    // initialize COM
    hr = CoInitialize(NULL);
    if (FAILED(hr))
        return FALSE;

    // create/initialize DirectInput interface
    hr = CoCreateInstance(
        &CLSID_DirectInput_bugs,
        NULL,
        CLSCTX_INPROC_SERVER,
        &IID_IDirectInputA_bugs,
        (void **)&dinput
    );
    if (FAILED(hr))
        return FALSE;
    hr = IDirectInput_Initialize(dinput, hInstance, DIRECTINPUT_VERSION);
    if (FAILED(hr))
        return FALSE;

    // create device for system keyboard
    kbdGuid = GUID_SysKeyboard_bugs;
    hr = IDirectInput_CreateDevice(dinput, &kbdGuid, &keyboardDev, NULL);
    if (SUCCEEDED(hr))
    {
        // set standard keyboard format
        IDirectInputDevice_SetDataFormat(keyboardDev, &c_dfDIKeyboard_bugs);
        // link keyboard to game window
        IDirectInputDevice_SetCooperativeLevel(
            keyboardDev,
            hWndGame,
            DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
        );

        // also try to acquire the keyboard
        hr = IDirectInputDevice_Acquire(keyboardDev);
        kbdAcquired = SUCCEEDED(hr);
    }
    else
    {
        // don't report failure to create keyboard, since input can fall back to
        // using Windows messages
        keyboardDev = NULL;
    }

    // search for an optional (attached) joystick and set it up if found
    IDirectInput_EnumDevices(
        dinput,
        DIDEVTYPE_JOYSTICK,
        EnumDInputDevs,
        NULL,
        DIEDFL_ATTACHEDONLY
    );

    return TRUE;
}

void FiniDInput(void)
{
    // joystick cleanup
    if (joystickDev)
    {
        IDirectInputDevice2_Unacquire(joystickDev);
        IDirectInputDevice2_Release(joystickDev);
        joystickDev = NULL;
    }

    // keyboard cleanup
    if (kbdUsesDInput)
    {
        if (kbdAcquired)
        {
            IDirectInputDevice2_Unacquire(keyboardDev);
            kbdAcquired = FALSE;
        }
        if (keyboardDev)
        {
            IDirectInputDevice2_Release(keyboardDev);
            keyboardDev = NULL;
        }
    }

    // DirectInput interface cleanup
    if (dinput)
        IDirectInput_Release(dinput);
    dinput = NULL;

    // cleanup COM
    CoUninitialize();
}

void InitInput(void)
{
    // only DirectInput really needs explicit init
    InitDInput();
}
