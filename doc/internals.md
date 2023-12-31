
This file documents various internal aspects about the game, such as file
formats, game options, and known IDs.

Decompiled code should be documented to the extent of its known usage in the
header/source files, not here.

# Launch Options

All options may be specified in all uppercase or all lowercase. For options with
values, there's no space between the option and the value.

- /b`N` - skips the main menu and starts immediately in level ID `N` (see
the [level list](#level-list) for IDs); e.g. `/b51` to go straight to "Nowhere"
- /p:`?` - unknown
- /r:`?` - unknown
- /x`N` - sets horizontal resolution, in pixels
- /y`N` - sets vertical resolution, in pixels
- /no_sync - disables V-sync?
- /no_skip - unknown; opposite of `/skip_intro`?
- /full - starts in fullscreen mode
- /win - starts in windowed mode
- /ogl, /opengl - selects hardware (OpenGL) renderer
- /soft, /softrgb, /soft16, /soft24 - selects 24-bit color software renderer
- /soft8, /softpal - selects 8-bit color software renderer
- /skip_intro - skips the first 3 startup screens
- /cons - unknown
- /fntp - unknown
- /dll:`S` - sets the name of the OpenGL library; default is "opengl32.dll"
- /pal - unknown
- /pal:`?` - unknown
- /? - unknown
- /q`N` - unknown
- /sam`N` - unknown
- /r`N` - unknown
- /o`N` - unknown
- /w`N` - unknown
- /n - unknown
- /sort`N` - unknown
- /lum`N` - unknown
- /sat`N` - unknown
- /mul`N` - unknown
- /gam`N` - unknown

# config.pc

This binary file contains the game's settings. It is 128 bytes and is directly
loaded into memory, so the in-memory structure matches the file layout.

| offset | type     | usage         | notes     |
|--------|----------|---------------|-----------|
| 0x00   | int      |               |           |
| 0x04   | int      | renderer      | see below |
| 0x08   | int      | language      | see below |
| 0x0c   | int      |               |           |
| 0x10   | int      | width         | pixels    |
| 0x14   | int      | height        | pixels    |
| 0x18   | bool32   | fullscreen    |           |
| 0x1c   | int      | fog depth     |           |
| 0x20   | double   |               |           |
| 0x28   | double   |               |           |
| 0x30   | int      | sound volume  | percent   |
| 0x34   | int      | music volume  | percent   |
| 0x38   | bool32   | joystick used |           |
| 0x3c   | byte[10] | joystick map  | see below |
| 0x46   | byte[16] | keyboard map  | see below |
| 0x56   | byte     |               |           |
| 0x57   | byte     |               |           |
| 0x58   | double   |               |           |
| 0x60   | double   |               |           |
| 0x68   | double   |               |           |
| 0x70   | double   |               |           |
| 0x78   | byte     |               |           |
| 0x79   | byte[7]  | padding       |           |

`renderer` choices:

- 0 - software renderer for "slow" computers (8-bit color)
- 1 - software renderer for "fast" computers (24-bit color)
- 2 - hardware (OpenGL) renderer

`language` codes:

- 0 - English
- 1 - French
- 2 - German
- 3 - Spanish
- 4 - Italian
- 5 - Dutch

joystick map TBD

keyboard map (entries are scancodes) TBD

# bugs.ini

This is a simple text file containing `Key=Value` lines. It is mostly used by
the vanilla launcher (BugsBunny.exe).

Known keys:

- Language - sets the language code in [`config.pc`](#config)
- FirstTime - used to detect when the game is launched for the first time; set
to 0 after first launch
- DLL - OpenGL DLL override, unless set to `0` (sets the `/DLL:` launch option)
- CheckGDI - whether to check for OpenGL acceleration (which affects available
renderer options)

# Level List

| ID  | filename   | description                                                          |
|-----|------------|----------------------------------------------------------------------|
| 0   | `title`    | main menu                                                            |
| 1   | `loading`  | generic LS (Loading Screen)                                          |
| 2   | `language` | file does not exist; language select                                 |
| 3   | `ls01`     | time period select                                                   |
| 4   | `l01a`     | _Wabbit on the run!_                                                 |
| 5   | `l01b`     | _Wabbit on the run!_                                                 |
| 6   | `l01c`     | _Guess who needs a kick start_                                       |
| 7   | `l01d1`    | _Magic Hare Blower_                                                  |
| 8   | `l01d2`    | _Magic Hare Blower_                                                  |
| 9   | `l02a1`    | _What's cookin', Doc?_                                               |
| 10  | `l02a2`    | _What's cookin', Doc?_                                               |
| 11  | `l02a3`    | _What's cookin', Doc?_                                               |
| 12  | `l02a4`    | _What's cookin', Doc?_                                               |
| 13  | `l02a5`    | _What's cookin', Doc?_                                               |
| 14  | `l02a6`    | _What's cookin', Doc?_                                               |
| 15  | `l02b1`    | _"Witch" way to Albuquerque?_                                        |
| 16  | `l02b2`    | _"Witch" way to Albuquerque?_                                        |
| 17  | `l02c1`    | _The Carrot-henge Mystery_                                           |
| 18  | `l02c2`    | _The Carrot-henge Mystery_                                           |
| 19  | `l02c3`    | _The Carrot-henge Mystery_                                           |
| 20  | `l02c4`    | _The Carrot-henge Mystery_                                           |
| 21  | `l03a`     | _Hey... What's up, Dock?_                                            |
| 22  | `l03acom`  | _Hey... What's up, Dock?_                                            |
| 23  | `l03a2`    | _Hey... What's up, Dock?_                                            |
| 24  | `l03b`     | _When Sam met Bunny_                                                 |
| 25  | `l03c`     | _Mine or mine?_                                                      |
| 26  | `l03c1`    | _Mine or mine?_                                                      |
| 27  | `l03c2`    | _Mine or mine?_                                                      |
| 28  | `l03d1`    | _Follow the Red Pirate Road_                                         |
| 29  | `l04a1`    | _The Big Bank Withdrawal_                                            |
| 30  | `l04a2`    | _The Big Bank Withdrawal_                                            |
| 31  | `l04a3`    | _The Big Bank Withdrawal_                                            |
| 32  | `l04b1`    | _The Greatest Escape_                                                |
| 33  | `l04b2`    | _The Greatest Escape_                                                |
| 34  | `l04b3`    | _The Greatest Escape_                                                |
| 35  | `l04c1`    | _The Carrot Factory_                                                 |
| 36  | `l04c2`    | _The Carrot Factory_                                                 |
| 37  | `l04c3`    | _The Carrot Factory_                                                 |
| 38  | `l04d1`    | _The Carrot Factory_                                                 |
| 39  | `l04d2`    | _The Carrot Factory_                                                 |
| 40  | `l04e`     | _Objects in the mirror are closer than they appear!_                 |
| 41  | `l04e2`    | _Objects in the mirror are closer than they appear!_                 |
| 42  | `l05a1`    | _The Planet X File!_                                                 |
| 43  | `l05a2`    | _The Planet X File!_                                                 |
| 44  | `l05a3a`   | _Train your Brain!_                                                  |
| 45  | `l05a3b`   | _Train your Brain!_                                                  |
| 46  | `l05a3c`   | _Train your Brain!_                                                  |
| 47  | `l05a4`    | _Train your Brain!_                                                  |
| 48  | `l05a5`    | _The Planet X File!_                                                 |
| 49  | `l05b1`    | _The Conquest for Planet X!_                                         |
| 50  | `l05c`     | _Vort "X" Room_                                                      |
| 51  | `merlin`   | _Nowhere_                                                            |
| 52  | `lb01`     | _Wabbit or Duck Season?_                                             |
| 53  | `lb02`     | _Downhill Duck_                                                      |
| 54  | `lb04`     | _La Corrida_                                                         |
| 55  | `lb05`     | file does not exist; cut bonus level for Dimension X?                |
| 56  | `ccintro`  | new game cutscene                                                    |
| 57  | `cc1a`     | IC (Intro Cutscene) for _Wabbit on the run!_                         |
| 58  | `cc2a`     | IC for _What's cookin', Doc?_                                        |
| 59  | `cc2b`     | IC for _"Witch" way to Albuquerque?_                                 |
| 60  | `cc2c`     | IC for _The Carrot-henge Mystery_                                    |
| 61  | `cc3a`     | IC for _Hey... What's up, Dock?_                                     |
| 62  | `cc3b`     | IC for _When Sam met Bunny_                                          |
| 63  | `cc3c`     | IC for _Mine or mine?_                                               |
| 64  | `cc3d`     | IC for _Follow the Red Pirate Road_                                  |
| 65  | `cc4a`     | IC for _The Big Bank Withdrawal_                                     |
| 66  | `cc4b`     | IC for _The Greatest Escape_                                         |
| 67  | `cc4c`     | IC for _The Carrot Factory_                                          |
| 68  | `cc4d`     | IC for _Objects in the mirror are closer than they appear!_          |
| 69  | `cc5a`     | IC for _The Planet X File!_                                          |
| 70  | `ccmerlin` | IC for _Nowhere_                                                     |
| 71  | `ccend`    | end game cutscene                                                    |
| 72  | `credits`  | game credits                                                         |
| 73  | `demo2a2`  | file does not exist; demo gameplay for main menu idle? (PS1 version) |
| 74  | `demo3c1`  | ditto                                                                |
| 75  | `demo4a1`  | ditto                                                                |
| 76  | `demo5c`   | ditto                                                                |
| 77  | `demob02`  | ditto                                                                |
| 78  | `screen1`  | first startup image                                                  |
| 79  | `screen2`  | second startup image                                                 |
| 80  | `screen3`  | third startup image                                                  |
| 81  | `l_30bank` | LS (Loading Screen) for _The Big Bank Withdrawal_                    |
| 82  | `l_30cf`   | LS for _The Carrot Factory_                                          |
| 83  | `l_30esca` | LS for _The Greatest Escape_                                         |
| 84  | `l_30load` | LS for _The 1930s_                                                   |
| 85  | `l_30ride` | LS for _Objects in the mirror are closer than they appear!_          |
| 86  | `l_bcorri` | LS for _La Corrida_                                                  |
| 87  | `l_bduck`  | LS for _Downhill Duck_                                               |
| 88  | `l_bhunt`  | LS for _Wabbit or Duck Season?_                                      |
| 89  | `l_fload`  | LS for _Dimension X_                                                 |
| 90  | `l_fload2` | LS for _The Conquest for Planet X!_                                  |
| 91  | `l_fride`  | LS for _Train your Brain!_                                           |
| 92  | `l_fxfile` | LS for _The Planet X File!_                                          |
| 93  | `l_fxroom` | LS for _Vort "X" Room_                                               |
| 94  | `l_mcook`  | LS for _What's cookin', Doc?_                                        |
| 95  | `l_mload`  | LS for _The Medieval Period_                                         |
| 96  | `l_mload2` | LS for _The Carrot-henge Mystery_                                    |
| 97  | `l_mwitch` | LS for _"Witch" way to Albuquerque?_                                 |
| 98  | `l_pcanon` | LS for _When Sam met Bunny_                                          |
| 99  | `l_pdock`  | LS for _Hey... What's up, Dock?_                                     |
| 100 | `l_pload`  | LS for _The Pirate Years_                                            |
| 101 | `l_pmine`  | LS for _Mine or mine?_                                               |
| 102 | `l_pred`   | LS for _Follow the Red Pirate Road_                                  |
| 103 | `l_sakick` | LS for _Guess who needs a kick start_                                |
| 104 | `l_saload` | LS for _The Stone Age_                                               |
| 105 | `l_sanigh` | LS for _Magic Hare Blower_                                           |
| 106 | `l_sarun`  | LS for _Wabbit on the run!_                                          |
| 107 | `l_merlin` | LS for _Nowhere_                                                     |
| 108 | `l_thall`  | LS for end                                                           |
| 109 | `screen4`  | file does not exist                                                  |
| 110 | `screen5`  | file does not exist                                                  |

# Level Format

The BZE file format is detailed [here](bze.md).
