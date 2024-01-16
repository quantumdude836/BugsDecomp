
// Keyboard scancodes.

#pragma once


// scancodes seem to be based off PS/2 scan code set 1, but where bit 7
// indicates an E0 key rather than make vs break
#define SK_ESCAPE   0x01
#define SK_TAB      0x0f
#define SK_Q        0x10
#define SK_W        0x11
#define SK_E        0x12
#define SK_R        0x13
#define SK_RETURN   0x1c
#define SK_A        0x1e
#define SK_S        0x1f
#define SK_D        0x20
#define SK_F        0x21
#define SK_SPACE    0x39
#define SK_UP       0xc8
#define SK_LEFT     0xcb
#define SK_RIGHT    0xcd
#define SK_DOWN     0xd0
