
#include "stdafx.h"
#include "common.h"
#include "pad.h"
#include "pad_prv.h"
#include "cfg.h"
#include "misc.h"
#include "scancodes.h"


// DirectInput data formats
#define c_dfDIJoystick_bugs (*(const DIDATAFORMAT *)VER(0x44ed80, 0x44ebe0))
#define c_dfDIKeyboard_bugs (*(const DIDATAFORMAT *)VER(0x44fda0, 0x44fc00))

// DirectInput IDs
#define IID_IDirectInputDevice2A_bugs (*(const IID *)0x45cc98)
#define GUID_SysKeyboard_bugs (*(const GUID *)0x45cca8)
#define CLSID_DirectInput_bugs (*(const CLSID *)0x45ccb8)
#define IID_IDirectInputA_bugs (*(const IID *)0x45ccc8)

// key name array
#define keyInfo (*(KEY_INFO (*)[128])0x45e030)

// last read joystick state
#define inXAxis (*(long *)0x4b1860)
#define inYAxis (*(long *)0x4b1864)
#define inButtons (*(int *)0x4b1878)

// DirectInput instance
#define dinput (*(LPDIRECTINPUTA *)0x4b18c0)

// DI device for joystick
#define joystickDev (*(LPDIRECTINPUTDEVICE2A *)0x4b18c4)

// DI device for keyboard
#define keyboardDev (*(LPDIRECTINPUTDEVICEA *)0x4b18c8)

// destination pointers for emulated PSX gamepads
#define pad1Dst (*(BYTE **)0x4b18d8)
#define pad2Dst (*(BYTE **)0x4b18dc)

// buffer used for GetButtonName
#define btnNameBuf (*(char (*)[96])0x9ca740)

// whether keyboard DI device is currently acquired
#define kbdAcquired (*(BOOL *)0x9ca8a0)

// current keyboard state
#define kbdState (*(BYTE (*)[256])0x9ca8c0)

// current keyboard read function
#define kbdReadFn (*(KBD_READ_FN *)0x9ca9c0)

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

const char *GetKeyName(int scancode)
{
    if (scancode == -1)
        return NULL;

    for (int i = 0; keyInfo[i].scancode != -1; i++)
    {
        if (keyInfo[i].scancode == scancode)
            return keyInfo[i].name;
    }

    return NULL;
}

void MapAltKeyboardInput(void)
{
    // read keyboard input
    kbdReadFn();

    // if no destination, early out now
    if (!pad1Dst)
        return;

    // un-invert button masks
    pad1Dst[2] = ~pad1Dst[2];
    pad1Dst[3] = ~pad1Dst[3];

    // map keys to buttons
    // only modify button masks if the corresponding key is pressed (don't
    // "release" any currently-pressed buttons)
    if (kbdState[SK_UP])
        pad1Dst[2] |= 1 << 4; // up
    if (kbdState[SK_RIGHT])
        pad1Dst[2] |= 1 << 5; // right
    if (kbdState[SK_DOWN])
        pad1Dst[2] |= 1 << 6; // down
    if (kbdState[SK_LEFT])
        pad1Dst[2] |= 1 << 7; // left
    if (kbdState[SK_TAB])
        pad1Dst[2] |= 1 << 0; // select
    if (kbdState[SK_RETURN])
        pad1Dst[2] |= 1 << 3; // start
    if (kbdState[config.kbdMap[4]]) // camera right
        pad1Dst[3] |= 1 << 0; // L2
    if (kbdState[config.kbdMap[5]]) // camera left
        pad1Dst[3] |= 1 << 1; // R2
    if (kbdState[config.kbdMap[6]]) // roll
        pad1Dst[3] |= 1 << 2; // L1
    if (kbdState[config.kbdMap[7]]) // action
        pad1Dst[3] |= 1 << 3; // R1
    if (kbdState[config.kbdMap[8]]) // look
        pad1Dst[3] |= 1 << 4; // triangle
    if (kbdState[config.kbdMap[9]]) // crouch
        pad1Dst[3] |= 1 << 5; // circle
    if (kbdState[config.kbdMap[10]]) // jump
        pad1Dst[3] |= 1 << 6; // cross/X
    if (kbdState[config.kbdMap[11]]) // kick
        pad1Dst[3] |= 1 << 7; // square

    // set terminal type to 4 (16-button) and data size to 1 (2 bytes)
    pad1Dst[1] = (4 << 4) | (2 >> 1);

    // re-invert button masks
    pad1Dst[2] = ~pad1Dst[2];
    pad1Dst[3] = ~pad1Dst[3];
}

void MapKeyboardInput(void)
{
    extraInputKeys = 0;

    // read keyboard input
    kbdReadFn();

    // if no destination, early out now
    if (!pad1Dst)
        return;

    // un-invert button masks
    pad1Dst[2] = ~pad1Dst[2];
    pad1Dst[3] = ~pad1Dst[3];
    BYTE oldButtonsHi = pad1Dst[2];

    // map keys to buttons
    // only modify button masks if the corresponding key is pressed (don't
    // "release" any currently-pressed buttons)
    if (kbdState[config.kbdMap[0]]) // up/forward
        pad1Dst[2] |= 1 << 4; // up
    if (kbdState[config.kbdMap[2]]) // right
        pad1Dst[2] |= 1 << 5; // right
    if (kbdState[config.kbdMap[1]]) // down/backward
        pad1Dst[2] |= 1 << 6; // down
    if (kbdState[config.kbdMap[3]]) // left
        pad1Dst[2] |= 1 << 7; // left
    if (kbdState[config.kbdMap[14]])
        pad1Dst[2] |= 1 << 0; // select
    if (kbdState[config.kbdMap[15]])
        pad1Dst[2] |= 1 << 3; // start
    if (kbdState[config.kbdMap[4]]) // camera right
        pad1Dst[3] |= 1 << 0; // L2
    if (kbdState[config.kbdMap[5]]) // camera left
        pad1Dst[3] |= 1 << 1; // R2
    if (kbdState[config.kbdMap[6]]) // roll
        pad1Dst[3] |= 1 << 2; // L1
    if (kbdState[config.kbdMap[7]]) // action
        pad1Dst[3] |= 1 << 3; // R1
    if (kbdState[config.kbdMap[8]]) // look
        pad1Dst[3] |= 1 << 4; // triangle
    if (kbdState[config.kbdMap[9]]) // crouch
        pad1Dst[3] |= 1 << 5; // circle
    if (kbdState[config.kbdMap[10]]) // jump
        pad1Dst[3] |= 1 << 6; // cross/X
    if (kbdState[config.kbdMap[11]]) // kick
        pad1Dst[3] |= 1 << 7; // square

    // also read extra input keys
    if (kbdState[SK_RETURN])
        extraInputKeys |= 1 << 0;
    if (kbdState[SK_ESCAPE])
        extraInputKeys |= 1 << 1;
    if (kbdState[SK_SPACE])
        extraInputKeys |= 1 << 2;

    // set terminal type to 4 (16-button) and data size to 1 (2 bytes), but only
    // if buttons 8-15 have changed
    if (pad1Dst[2] != oldButtonsHi)
        pad1Dst[1] = (4 << 4) | (2 >> 1);

    // re-invert button masks
    pad1Dst[2] = ~pad1Dst[2];
    pad1Dst[3] = ~pad1Dst[3];
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

void ReadKbdDInput(void)
{
    HRESULT hr;

    // read state
    hr = IDirectInputDevice_GetDeviceState(
        keyboardDev,
        sizeof kbdState,
        kbdState
    );
    if (hr == S_OK)
        return;

    // try to reacquire keyboard
    // FIXME: verify correctness
    while (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
    {
        kbdAcquired = FALSE;
        hr = IDirectInputDevice_Acquire(keyboardDev);
        if (FAILED(hr))
            break;

        hr = IDirectInputDevice_GetDeviceState(
            keyboardDev,
            sizeof kbdState,
            kbdState
        );
        if (hr == S_OK)
            return;
    }

    // unable to read via DirectInput; fallback to Windows messages
    ReadKbdWinMsg();
}

void ReadKbdWinMsg(void)
{
    static const struct
    {
        BYTE kbdKey;
        BYTE winKey;
    } keyMap[] = {
        { 0x01, 0x1b },
        { 0x3b, 0x70 },
        { 0x3c, 0x71 },
        { 0x3d, 0x72 },
        { 0x3e, 0x73 },
        { 0x3f, 0x74 },
        { 0x40, 0x75 },
        { 0x41, 0x76 },
        { 0x42, 0x77 },
        { 0x43, 0x78 },
        { 0x44, 0x79 },
        { 0x57, 0x7a },
        { 0x58, 0x7b },
        { 0x0b, 0x30 },
        { 0x02, 0x31 },
        { 0x03, 0x32 },
        { 0x04, 0x33 },
        { 0x05, 0x34 },
        { 0x06, 0x35 },
        { 0x07, 0x36 },
        { 0x08, 0x37 },
        { 0x09, 0x38 },
        { 0x0a, 0x39 },
        { 0x10, 0x51 },
        { 0x11, 0x57 },
        { 0x12, 0x45 },
        { 0x13, 0x52 },
        { 0x14, 0x54 },
        { 0x15, 0x59 },
        { 0x16, 0x55 },
        { 0x17, 0x49 },
        { 0x18, 0x4f },
        { 0x19, 0x50 },
        { 0x1e, 0x41 },
        { 0x1f, 0x53 },
        { 0x20, 0x44 },
        { 0x21, 0x46 },
        { 0x22, 0x47 },
        { 0x23, 0x48 },
        { 0x24, 0x4a },
        { 0x25, 0x4b },
        { 0x26, 0x4c },
        { 0x2c, 0x5a },
        { 0x2d, 0x58 },
        { 0x2e, 0x43 },
        { 0x2f, 0x56 },
        { 0x30, 0x42 },
        { 0x31, 0x4e },
        { 0x32, 0x4d },
        { 0x39, 0x20 },
        { 0x47, 0x67 },
        { 0x48, 0x68 },
        { 0x49, 0x69 },
        { 0x4b, 0x64 },
        { 0x4c, 0x65 },
        { 0x4d, 0x66 },
        { 0x4f, 0x61 },
        { 0x50, 0x62 },
        { 0x51, 0x63 },
        { 0x52, 0x60 },
        { 0x37, 0x6a },
        { 0x0f, 0x09 },
        { 0x1d, 0xa2 },
        { 0x2a, 0xa0 },
        { 0x36, 0xa1 },
        { 0x38, 0xa4 },
        { 0x3a, 0x14 },
        { 0x45, 0x90 },
        { 0x46, 0x91 },
        { 0x0e, 0x08 },
        { 0x4a, 0x6d },
        { 0x4e, 0x6b },
        { 0x1c, 0x0d },
        { 0x53, 0x6e },
        { 0x9c, 0x0d },
        { 0x9d, 0xa3 },
        { 0xb5, 0x6f },
        { 0xb7, 0x2c },
        { 0xb8, 0xa5 },
        { 0xc7, 0x24 },
        { 0xc8, 0x26 },
        { 0xc9, 0x21 },
        { 0xcb, 0x25 },
        { 0xcd, 0x27 },
        { 0xcf, 0x23 },
        { 0xd0, 0x28 },
        { 0xd1, 0x22 },
        { 0xd2, 0x2d },
        { 0xd3, 0x2e },
    };

    BYTE winKeyState[256];
    MSG msg;

    // pump all messages to get latest keyboard state
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            dword_45F2B0 = 0;
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    if (!GetKeyboardState(winKeyState))
        ConPrintf(BSTR(0x45f230, "***********keyboard handler error***********\n"));

    // translate keys
    for (int i = 0; i < ARRAYSIZE(keyMap); i++)
    {
        // (Windows) key state is stored in bit 7
        kbdState[keyMap[i].kbdKey] = winKeyState[keyMap[i].winKey] >> 7;
    }

}

void PadStartCom(void)
{
    // emulated controller input uses DirectInput
    InitDInput();
}

int PadGetState(int port)
{
    // don't emulate any state other than stable
    return PadStateStable;
}

int PadInfoMode(int port, int term, int offs)
{
    return 0;
}

int PadSetActAlign(int port, BYTE *data)
{
    return 0;
}

int PadSetMainMode(int port, int offs, int lock)
{
    return 0;
}

void PadSetAct(int port, BYTE *data, int len)
{
}

void PadInitDirect(BYTE *pad1, BYTE *pad2)
{
    pad1Dst = pad1;
    pad2Dst = pad2;
}

void MapJoystickInput(void)
{
    // if no destination, early out now
    if (!pad1Dst)
        return;

    // initialize report to defaults
    pad1Dst[0] = 0; // no error
    // default to terminal type 4 (16-button), 2-byte input
    pad1Dst[1] = (4 << 4) | (2 >> 1);
    // all buttons released
    pad1Dst[2] = 0xff;
    pad1Dst[3] = 0xff;
    // go ahead and set axes to neutral
    pad1Dst[4] = 128;
    pad1Dst[5] = 128;
    pad1Dst[6] = 128;
    pad1Dst[7] = 128;

    // try to read joystick input; bail out if unable to
    if (!ReadJoystick(&inXAxis, &inYAxis, &inButtons))
        return;

    // re-map axis bounds
    inXAxis += 128;
    if ((unsigned)inXAxis > 255)
        inXAxis = 255;
    inYAxis += 128;
    if ((unsigned)inYAxis > 255)
        inYAxis = 255;

    // change to terminal type 7 (analog controller), 6-byte input
    pad1Dst[1] = (7 << 4) | (6 >> 1);

    // map buttons, reusing original input variable for extra buttons
    int buttons = inButtons;
    inButtons = 0;
    pad1Dst[3] = 0;
    if (buttons & (1 << config.joyMap[0])) // look
        pad1Dst[3] |= 1 << 4; // triangle
    if (buttons & (1 << config.joyMap[1])) // crouch
        pad1Dst[3] |= 1 << 5; // circle
    if (buttons & (1 << config.joyMap[2])) // jump
        pad1Dst[3] |= 1 << 6; // cross/X
    if (buttons & (1 << config.joyMap[3])) // kick
        pad1Dst[3] |= 1 << 7; // square
    if (buttons & (1 << config.joyMap[4])) // roll
        pad1Dst[3] |= 1 << 2; // L1
    if (buttons & (1 << config.joyMap[5])) // action
        pad1Dst[3] |= 1 << 3; // R1
    if (buttons & (1 << config.joyMap[6])) // camera left
        pad1Dst[3] |= 1 << 0; // L2
    if (buttons & (1 << config.joyMap[7])) // camera right
        pad1Dst[3] |= 1 << 1; // R2
    if (buttons & (1 << config.joyMap[8]))
        inButtons |= 1 << 8;
    if (buttons & (1 << config.joyMap[9]))
        inButtons |= 1 << 9;

    // the left shift by 2 is odd
    pad1Dst[2] = ((inButtons >> 8) & 0x0f) << 2;

    // map axes to arrow buttons
    if ((BYTE)inYAxis < 16)
        pad1Dst[2] |= 1 << 4; // up
    if ((BYTE)inXAxis > (256 - 16))
        pad1Dst[2] |= 1 << 5; // right
    if ((BYTE)inYAxis > (256 - 16))
        pad1Dst[2] |= 1 << 6; // down
    if ((BYTE)inXAxis < 16)
        pad1Dst[2] |= 1 << 7; // left

    // (re-)invert buttons
    pad1Dst[2] = ~pad1Dst[2];
    pad1Dst[3] = ~pad1Dst[3];

    // copy axes; since only one X/Y pair is read, write them to both PSX pairs
    pad1Dst[4] = (BYTE)inXAxis;
    pad1Dst[5] = (BYTE)inYAxis;
    pad1Dst[6] = (BYTE)inXAxis;
    pad1Dst[7] = (BYTE)inYAxis;
}

int GetPressedButton(void)
{
    BOOL res = ReadJoystick(&inXAxis, &inYAxis, &inButtons);
    if (!res)
        return 0xff;

    for (int button = 0; button < 16; button++)
    {
        if (inButtons & (1 << button))
            return button;
    }

    return 0xfe;
}

const char *GetButtonName(int button)
{
    BOOL valid = FALSE;

    if (button <= 16)
    {
        sprintf(btnNameBuf, BSTR(0x45f26c, "BUTTON%i"), button);
        valid = TRUE;
    }
    if (button == 0xff)
    {
        strcpy(btnNameBuf, BSTR(0x45f264, "NULL"));
        valid = TRUE;
    }
    if (button == 0xfe)
    {
        strcpy(btnNameBuf, BSTR(0x45f260, " "));
        valid = TRUE;
    }
    return valid ? btnNameBuf : NULL;
}
