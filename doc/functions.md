
The following table provides details about known functions:

| version 1.0 address | version 1.6 address | name              | decompiled? |
|---------------------|---------------------|-------------------|-------------|
| 0x401000            | 0x401000            | InitTrack         | yes         |
| 0x401330            | 0x401330            | FiniTrack         | yes         |
| 0x4013c0            | 0x4013c0            | SetTrackSource    | yes         |
| 0x401490            | 0x401490            | ResetTrack        | yes         |
| 0x4014f0            | 0x4014f0            | RefillTrackBuffer | yes         |
| 0x4018d0            | 0x4018d0            | PlayTrack         | yes         |
| 0x401910            | 0x401910            | StopTrack         | yes         |
| 0x401930            | 0x401930            | CheckTrackDone    | yes         |
| 0x4019a0            | 0x4019a0            | UpdateTrack       | yes         |
| 0x401af0            | 0x401af0            | ConvertTrackAudio | yes         |
| 0x401be0            | 0x401be0            | CvtStereoAdpcm    | yes         |
| 0x401e10            | 0x401e10            | CvtMonoAdpcm      | yes         |
| 0x402000            | 0x402000            | EnumDInputDevs    | yes         |
| 0x402180            | 0x402180            | ReadJoystick      | yes         |
| 0x402250            | 0x402250            | GetKeyName        | yes         |
| 0x402610            | 0x402610            | InitDInput        | yes         |
| 0x402790            | 0x402790            | FiniDInput        | yes         |
| 0x402810            | 0x402810            | ReadKbdDInput     | yes         |
| 0x4028a0            | 0x4028a0            | ReadKbdWinMsg     | yes         |
| 0x402fd0            | 0x402fd0            | InitInput         | yes         |
| 0x403260            | 0x403260            | GetButtonName     | yes         |
| 0x4055d0            | 0x4055d0            | ConPrintf         | yes         |
| 0x405640            | 0x405640            | ShowRetryDialog   | yes         |
| 0x4056c0            | 0x4056c0            | FsOpen            | yes         |
| 0x405760            | 0x405760            | FsFOpen           | yes         |
| 0x405800            | 0x405800            | FsSeek            | yes         |
| 0x405820            | 0x405820            | FsRead            | yes         |
| 0x405840            | 0x405840            | FsClose           | yes         |
| 0x405850            | 0x405850            | FsFindRoot        | yes         |
| 0x4079e0            | 0x4079c0            | SquareRoot0       | yes         |
| 0x407a00            | 0x4079e0            | ratan2            | yes         |
| 0x407a30            | 0x407a10            | rcos              | yes         |
| 0x407a50            | 0x407a30            | rsin              | yes         |
| 0x407a70            | 0x407a50            | RotMatrix         | yes         |
| 0x407b10            | 0x407af0            | RotMatrixYXZ      | yes         |
| 0x407bb0            | 0x407b90            | RotMatrixZYX      | yes         |
| 0x407c50            | 0x407c30            | RotMatrixX        | yes         |
| 0x407dc0            | 0x407da0            | RotMatrixY        | yes         |
| 0x407f20            | 0x407f00            | RotMatrixZ        | yes         |
| 0x408080            | 0x408060            | ScaleMatrix       | yes         |
| 0x408120            | 0x408100            | TransMatrix       | yes         |
| 0x408140            | 0x408120            | ApplyMatrix       | yes         |
| 0x4081e0            | 0x4081c0            | ApplyMatrixSV     | yes         |
| 0x408280            | 0x408260            | ApplyMatrixLV     | yes         |
| 0x408310            | 0x4082f0            | MulMatrix0        | yes         |
| 0x4084b0            | 0x408490            | MulMatrix2        | yes         |
| 0x408660            | 0x408640            | MulMatrix2_0      | yes         |
| 0x4086b0            | 0x408690            | CompMatrixLV      | yes         |
| 0x4088d0            | 0x4088b0            | CompMatrix2LV     | yes         |
| 0x408a40            | 0x408a20            | IdentMatrix       | yes         |
| 0x408a80            | 0x408a60            | ApplyMatrix_0     | yes         |
| 0x408b20            | 0x408b00            | ApplyMatrixSV_0   | yes         |
| 0x408bc0            | 0x408ba0            | Square0           | yes         |
| 0x409a40            | 0x409a10            | WriteConfig       | yes         |
| 0x409ac0            | 0x409a90            | SetConfigDefaults | yes         |
| 0x409ce0            | 0x409c80            | SaveAltConfig     | yes         |
| 0x409d00            | 0x409ca0            | LoadAltConfig     | yes         |
| 0x409d20            | 0x409cc0            | ReadConfig        | yes         |
| 0x44fdd0            | 0x44fc30            | malloc            | no[^1]      |
| 0x44ff10            | 0x44fd70            | free              | no[^1]      |
| 0x44ff60            | 0x44fd60            | _lseek            | no[^1]      |
| 0x450020            | 0x44fe80            | _read             | no[^1]      |
| 0x450370            | 0x4501d0            | fseek             | no[^1]      |
| 0x450410            | 0x450270            | ftell             | no[^1]      |
| 0x4506a0            | 0x450500            | fclose            | no[^1]      |
| 0x450760            | 0x4505c0            | fwrite            | no[^1]      |
| 0x4508e0            | 0x450740            | fopen             | no[^1]      |
| 0x450a90            | 0x4508f0            | fread             | no[^1]      |
| 0x4513d0            | 0x451230            | _close            | no[^1]      |
| 0x458620            | 0x458480            | _open             | no[^1]      |

[^1]: C runtime function; will not be decompiled
