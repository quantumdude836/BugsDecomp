
The following table provides details about known functions across all known
game versions, including PSX (US):

| name                  | PC v1.0 addr     | PC v1.6 addr     | PSX US addr  | group | subgroup | decompiled? |
|-----------------------|------------------|------------------|--------------|-------|----------|-------------|
| `InitTrack`           | 0x401000         | 0x401000         | ?            | misc  | track    | yes         |
| `FiniTrack`           | 0x401330         | 0x401330         | ?            | misc  | track    | yes         |
| `SetTrackSource`      | 0x4013c0         | 0x4013c0         | ?            | misc  | track    | yes         |
| `ResetTrack`          | 0x401490         | 0x401490         | ?            | misc  | track    | yes         |
| `RefillTrackBuffer`   | 0x4014f0         | 0x4014f0         | ?            | misc  | track    | yes         |
| `PlayTrack`           | 0x4018d0         | 0x4018d0         | ?            | misc  | track    | yes         |
| `StopTrack`           | 0x401910         | 0x401910         | ?            | misc  | track    | yes         |
| `CheckTrackDone`      | 0x401930         | 0x401930         | ?            | misc  | track    | yes         |
| `UpdateTrack`         | 0x4019a0         | 0x4019a0         | ?            | misc  | track    | yes         |
| `ConvertTrackAudio`   | 0x401af0         | 0x401af0         | ?            | misc  | track    | yes         |
| `CvtStereoAdpcm`      | 0x401be0         | 0x401be0         | ?            | misc  | track    | yes         |
| `CvtMonoAdpcm`        | 0x401e10         | 0x401e10         | ?            | misc  | track    | yes         |
| `EnumDInputDevs`      | 0x402000         | 0x402000         |              | port  | pad      | yes         |
| `ReadJoystick`        | 0x402180         | 0x402180         |              | port  | pad      | yes         |
| `GetKeyName`          | 0x402250         | 0x402250         |              | port  | pad      | yes         |
| `MapAltKeyboardInput` | 0x402290         | 0x402290         |              | port  | pad      | yes         |
| `MapKeyboardInput`    | 0x4023e0         | 0x4023e0         |              | port  | pad      | yes         |
| `InitDInput`          | 0x402610         | 0x402610         |              | port  | pad      | yes         |
| `FiniDInput`          | 0x402790         | 0x402790         |              | port  | pad      | yes         |
| `ReadKbdDInput`       | 0x402810         | 0x402810         |              | port  | pad      | yes         |
| `ReadKbdWinMsg`       | 0x4028a0         | 0x4028a0         |              | port  | pad      | yes         |
| `PadStartCom`         | 0x402fd0         | 0x402fd0         | 0x8005714c   | sdk   | pad      | yes         |
| `PadGetState`         | 0x402fe0         | 0x402fe0         | 0x800571d8   | sdk   | pad      | yes         |
| `PadInfoMode`         | 0x408a70[^merge] | 0x408a50[^merge] | 0x800572a4   | sdk   | pad      | yes         |
| `PadSetActAlign`      | 0x408a70[^merge] | 0x408a50[^merge] | 0x80057518   | sdk   | pad      | yes         |
| `PadSetMainMode`      | 0x408a70[^merge] | 0x408a50[^merge] | 0x80057550   | sdk   | pad      | yes         |
| `PadSetAct`           | 0x42d400[^merge] | 0x417010[^merge] | 0x80057598   | sdk   | pad      | yes         |
| `PadInitDirect`       | 0x402ff0         | 0x402ff0         | 0x8005960c   | sdk   | pad      | yes         |
| `MapJoystickInput`    | 0x403010         | 0x403010         |              | port  | pad      | yes         |
| `GetPressedButton`    | 0x403210         | 0x403210         |              | port  | pad      | yes         |
| `GetButtonName`       | 0x403260         | 0x403260         |              | port  | pad      | yes         |
| `ConPrintf`           | 0x4055d0         | 0x4055d0         |              | port  | misc     | yes         |
| `ShowRetryDialog`     | 0x405640         | 0x405640         |              | port  | misc     | yes         |
| `FsOpen`              | 0x4056c0         | 0x4056c0         |              | port  | fs       | yes         |
| `FsFOpen`             | 0x405760         | 0x405760         |              | port  | fs       | yes         |
| `FsSeek`              | 0x405800         | 0x405800         |              | port  | fs       | yes         |
| `FsRead`              | 0x405820         | 0x405820         |              | port  | fs       | yes         |
| `FsClose`             | 0x405840         | 0x405840         |              | port  | fs       | yes         |
| `FsFindRoot`          | 0x405850         | 0x405850         |              | port  | fs       | yes         |
| `SquareRoot0`         | 0x4079e0         | 0x4079c0         | 0x8004f72c   | sdk   | gte      | yes         |
| `ratan2`              | 0x407a00         | 0x4079e0         | 0x800508dc   | sdk   | gte      | yes         |
| `rcos`                | 0x407a30         | 0x407a10         | 0x8004f5fc   | sdk   | gte      | yes         |
| `rsin`                | 0x407a50         | 0x407a30         | 0x8004f52c   | sdk   | gte      | yes         |
| `RotMatrix`           | 0x407a70         | 0x407a50         | 0x8004fc4c   | sdk   | gte      | yes         |
| `RotMatrixYXZ`        | 0x407b10         | 0x407af0         | 0x8004fedc   | sdk   | gte      | yes         |
| `RotMatrixZYX`        | 0x407bb0         | 0x407b90         | 0x8005016c   | sdk   | gte      | yes         |
| `RotMatrixX`          | 0x407c50         | 0x407c30         | 0x800503fc   | sdk   | gte      | yes         |
| `RotMatrixY`          | 0x407dc0         | 0x407da0         | 0x8005059c   | sdk   | gte      | yes         |
| `RotMatrixZ`          | 0x407f20         | 0x407f00         | 0x8005073c   | sdk   | gte      | yes         |
| `ScaleMatrix`         | 0x408080         | 0x408060         | 0x8004fb0c   | sdk   | gte      | yes         |
| `TransMatrix`         | 0x408120         | 0x408100         | 0x8004fadc   | sdk   | gte      | yes         |
| `ApplyMatrix`         | 0x408140         | 0x408120         | 0x8004fa2c   | sdk   | gte      | yes         |
| `ApplyMatrixSV`       | 0x4081e0         | 0x4081c0         | 0x8004fa7c   | sdk   | gte      | yes         |
| `ApplyMatrixLV`       | 0x408280         | 0x408260         | 0x8004f7bc   | sdk   | gte      | yes         |
| `MulMatrix0`          | 0x408310         | 0x4082f0         | 0x8005556c   | sdk   | gte      | yes         |
| `MulMatrix2`          | 0x4084b0         | 0x408490         | 0x8004f91c   | sdk   | gte      | yes         |
| `MulMatrix2_0`        | 0x408660         | 0x408640         |              | port  | gte      | yes         |
| `CompMatrixLV`        | 0x4086b0         | 0x408690         | ?            | sdk   | gte      | yes         |
| `CompMatrix2LV`       | 0x4088d0         | 0x4088b0         |              | port  | gte      | yes         |
| `IdentMatrix`         | 0x408a40         | 0x408a20         |              | port  | gte      | yes         |
| `gte_ApplyMatrix`     | 0x408a80         | 0x408a60         | none[^macro] | sdk   | gte      | yes         |
| `gte_ApplyMatrixSV`   | 0x408b20         | 0x408b00         | none[^macro] | sdk   | gte      | yes         |
| `Square0`             | 0x408bc0         | 0x408ba0         | 0x80056ef4   | sdk   | gte      | yes         |
| `WriteConfig`         | 0x409a40         | 0x409a10         |              | port  | cfg      | yes         |
| `SetConfigDefaults`   | 0x409ac0         | 0x409a90         |              | port  | cfg      | yes         |
| `SaveAltConfig`       | 0x409ce0         | 0x409c80         |              | port  | cfg      | yes         |
| `LoadAltConfig`       | 0x409d00         | 0x409ca0         |              | port  | cfg      | yes         |
| `ReadConfig`          | 0x409d20         | 0x409cc0         |              | port  | cfg      | yes         |
| `GsSetProjection`     | 0x40cd80         | 0x40cd20         | 0x80055afc   | sdk   | gs       | no          |
| `GsMapModelingData`   | 0x40cdc0         | 0x40cd60         | 0x80055a8c   | sdk   | gs       | no          |
| `GsInitCoordinate2`   | 0x40ce90         | 0x40ce30         | 0x800551ac   | sdk   | gs       | no          |
| `GsSetRefView2L`      | 0x40cf10         | 0x40ceb0         | 0x8005661c   | sdk   | gs       | no          |
| `SetGLPixelFormat`    | 0x40df80         | 0x40df20         |              | port  | rogl     | yes         |
| `TestGL`              | 0x40e0b0         | 0x40e050         |              | port  | rogl     | yes         |
| `UnloadGL`            | 0x40e210         | 0x40e1b0         |              | port  | rogl     | yes         |
| `TestLogicCond`       | 0x42a850         | 0x42a7d0         | 0x8002f9f8   | game  | logic    | yes         |
| `RunLogicAction`      | 0x42c760         | 0x42c6d0         | 0x80030ecc   | game  | logic    | yes         |
| `AllocListNode`       | 0x4489c0         | 0x448850         | 0x80036204   | game  | list     | yes         |
| `InitListPool`        | 0x448a00         | 0x448890         | 0x80036250   | game  | list     | yes         |
| `RemoveListNode`      | 0x448a40         | 0x4488d0         | 0x800362d0   | game  | list     | yes         |
| `ClearList`           | 0x448a90         | 0x448920         | 0x80036358   | game  | list     | yes         |
| `AppendListNode`      | 0x448ac0         | 0x448950         | 0x8003638c   | game  | list     | yes         |
| `malloc`              | 0x44fdd0         | 0x44fc30         |              | crt   | crt      | no[^crt]    |
| `free`                | 0x44ff10         | 0x44fd70         |              | crt   | crt      | no[^crt]    |
| `_lseek`              | 0x44ff60         | 0x44fd60         |              | crt   | crt      | no[^crt]    |
| `_read`               | 0x450020         | 0x44fe80         |              | crt   | crt      | no[^crt]    |
| `fseek`               | 0x450370         | 0x4501d0         |              | crt   | crt      | no[^crt]    |
| `ftell`               | 0x450410         | 0x450270         |              | crt   | crt      | no[^crt]    |
| `fclose`              | 0x4506a0         | 0x450500         |              | crt   | crt      | no[^crt]    |
| `fwrite`              | 0x450760         | 0x4505c0         |              | crt   | crt      | no[^crt]    |
| `fopen`               | 0x4508e0         | 0x450740         |              | crt   | crt      | no[^crt]    |
| `fread`               | 0x450a90         | 0x4508f0         |              | crt   | crt      | no[^crt]    |
| `_close`              | 0x4513d0         | 0x451230         |              | crt   | crt      | no[^crt]    |
| `_open`               | 0x458620         | 0x458480         |              | crt   | crt      | no[^crt]    |

[^merge]: in many cases, functions that are equivalent were merged when linking
the final binary, thus they have the same address in PC versions
[^macro]: macro in the Psy-Q SDK, implemented as a function for PC port
[^crt]: C runtime function; will not be decompiled
