
This file documents various internal aspects about the game, such as file
formats, game options, and known IDs.

Decompiled code should be documented to the extent of its known usage in the
header/source files, not here.

# Launch Options

All options may be specified in all uppercase or all lowercase. For options with
values, there's no space between the option and the value.

- /b`N` - skips the main menu and starts immediately in level ID `N` (see
the [level list](#level-list) for IDs); e.g. `/b51` to go straight to "Nowhere";
defaults to 2, i.e. the (non-existent) language select level
- /p:`path` - loads data previously dumped by `/r:` from `path`
- /r:`path` - dumps some unknown data to `path` on game exit
- /x`N` - sets horizontal resolution, in pixels
- /y`N` - sets vertical resolution, in pixels
- /no_sync - unknown
- /no_skip - unknown; related to `/no_sync`
- /full - starts in fullscreen mode
- /win - starts in windowed mode
- /ogl, /opengl - selects hardware (OpenGL) renderer
- /soft, /softrgb, /soft16, /soft24 - selects 24-bit color software renderer
- /soft8, /softpal - selects 8-bit color software renderer
- /skip_intro - skips the first 3 startup screens
- /cons - unknown; appears to be unused
- /fntp - unknown; appears to be unused
- /dll:`name` - sets the name of the OpenGL library; default is "opengl32.dll"
- /pal - unknown; `/b<N>` must be specified first; dumps a palette BMP of the
selected level?
- /pal:`name` - unknown; `name` is relative to "bze" directory of BBLiT; seems
to be bugged (relying on an uninitialized variable)
- /? - unknown
- /q`N` - unknown; legal values appear to be 0,1,2
- /sam`N` - unknown; appears to be unused
- /r`N` - unknown; not to be confused with `/r:`; legal values appear to be
(0,)1,2,3,4,5,8
- /o`N` - unknown; appears to be unused
- /w`N` - unknown; legal values appear to be (0,)1,2,3
- /n - unknown
- /sort`N` - unknown; appears to be unused
- /lum`N` - unknown; value scaled by 1/100
- /sat`N` - unknown; value scaled by 1/100
- /mul`N` - unknown; value scaled by 1/100
- /gam`N` - unknown; value scaled by 1/100

# Config Files

## config.pc

This binary file contains the game's settings. It is 128 bytes and is directly
loaded into memory, so the in-memory structure matches the file layout.

| offset | type     | usage         | notes     |
|--------|----------|---------------|-----------|
| 0x00   | int      | version       | = 0x10    |
| 0x04   | int      | renderer      | see below |
| 0x08   | int      | language      | see below |
| 0x0c   | int      | display type? | see below |
| 0x10   | int      | width         | pixels    |
| 0x14   | int      | height        | pixels    |
| 0x18   | bool32   | fullscreen    |           |
| 0x1c   | int      | fog distance  | see below |
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

`renderer` values:

- 0 - software renderer for "slow" computers (8-bit color)
- 1 - software renderer for "fast" computers (24-bit color)
- 2 - hardware (OpenGL) renderer

`language` values:

- 0 - English
- 1 - French
- 2 - German
- 3 - Spanish
- 4 - Italian
- 5 - Dutch

`display type` values:

- 0 - unknown
- 1 - unknown
- 2 - unknown

`fog distance` values:

- 0 - near
- 1 - normal
- 2 - far

joystick map (entries are joystick buttons):

| index | usage        |
|-------|--------------|
| 0     | look         |
| 1     | crouch       |
| 2     | jump         |
| 3     | kick         |
| 4     | roll         |
| 5     | action       |
| 6     | camera left  |
| 7     | camera right |
| 8     | unknown      |
| 9     | unknown      |

keyboard map (entries are scancodes):

| index | usage         |
|-------|---------------|
| 0     | up/forward    |
| 1     | down/backward |
| 2     | right         |
| 3     | left          |
| 4     | camera right  |
| 5     | camera left   |
| 6     | roll          |
| 7     | action        |
| 8     | look          |
| 9     | crouch        |
| 10    | jump          |
| 11    | kick          |
| 12    | unknown       |
| 13    | unknown       |
| 14    | PSX select    |
| 15    | PSX start     |

## bugs.ini

This is a simple text file containing `Key=Value` lines. It is mostly used by
the vanilla launcher (BugsBunny.exe).

Known keys:

- Language - sets the language code in [`config.pc`](#config)
- FirstTime - used to detect when the game is launched for the first time; set
to 0 after first launch
- DLL - OpenGL DLL override, unless set to `0` (sets the `/DLL:` launch option)
- CheckGDI - whether to check for OpenGL acceleration (which affects available
renderer options)

# Levels

## Level List

| ID  | filename   | description                                                          |
|-----|------------|----------------------------------------------------------------------|
| 0   | `title`    | main menu                                                            |
| 1   | `loading`  | generic LS (Loading Screen)                                          |
| 2   | `language` | file does not exist; language select                                 |
| 3   | `ls01`     | time period select                                                   |
| 4   | `l01a`     | _Wabbit on the run!_; _Dinosaur Mountain_; level start               |
| 5   | `l01b`     | _Wabbit on the run!_; _Pterodactyl Cliff_; level end                 |
| 6   | `l01c`     | _Guess who needs a kick start_                                       |
| 7   | `l01d1`    | _Magic Hare Blower_; _Dinosaur Mountain_; level start                |
| 8   | `l01d2`    | _Magic Hare Blower_; _Pterodactyl Cliff_; level end                  |
| 9   | `l02a1`    | _What's cookin', Doc?_; _King's Fields_; level start                 |
| 10  | `l02a2`    | _What's cookin', Doc?_; _Royal Square_/_Spiral Tower_                |
| 11  | `l02a3`    | _What's cookin', Doc?_; _Royal Apple Tree Gardens_                   |
| 12  | `l02a4`    | _What's cookin', Doc?_; _Ramparts_                                   |
| 13  | `l02a5`    | _What's cookin', Doc?_; _Forgotten Woods_                            |
| 14  | `l02a6`    | _What's cookin', Doc?_; level end                                    |
| 15  | `l02b1`    | _"Witch" way to Albuquerque?_; _Hazel's Hill_; level start           |
| 16  | `l02b2`    | _"Witch" way to Albuquerque?_; _Windmill Road_; level end            |
| 17  | `l02c1`    | _The Carrot-henge Mystery_; _Frozen Duck valley_; level start        |
| 18  | `l02c2`    | _The Carrot-henge Mystery_; _Carrot-henge_                           |
| 19  | `l02c3`    | _The Carrot-henge Mystery_; _Robin Duck's Lair_/_Slide Hare_         |
| 20  | `l02c4`    | _The Carrot-henge Mystery_; _Zee Cavern_/_Raquette Hare!_            |
| 21  | `l03a`     | _Hey... What's up, Dock?_; _The Docks_; level start                  |
| 22  | `l03acom`  | _Hey... What's up, Dock?_; level end                                 |
| 23  | `l03a2`    | _Hey... What's up, Dock?_; _Shark Islands_                           |
| 24  | `l03b`     | _When Sam met Bunny_                                                 |
| 25  | `l03c`     | _Mine or mine?_; _Pirate's Cove_; level start                        |
| 26  | `l03c1`    | _Mine or mine?_; _Sam' S Mine. No Tresspassers!_                     |
| 27  | `l03c2`    | _Mine or mine?_; level end                                           |
| 28  | `l03d1`    | _Follow the Red Pirate Road_                                         |
| 29  | `l04a1`    | _The Big Bank Withdrawal_; _Bank Basement_; level start              |
| 30  | `l04a2`    | _The Big Bank Withdrawal_; _Inside the Bank_                         |
| 31  | `l04a3`    | _The Big Bank Withdrawal_; _Bank Rooftop_; level end                 |
| 32  | `l04b1`    | _The Greatest Escape_; _Hotel Hall_; level start                     |
| 33  | `l04b2`    | _The Greatest Escape_; _First Floor_/_Second Floor_                  |
| 34  | `l04b3`    | _The Greatest Escape_; _Hotel Rooftop_; level end                    |
| 35  | `l04c1`    | _The Carrot Factory_; _Carrot Steamer_; level start                  |
| 36  | `l04c2`    | _The Carrot Factory_; _Carrot Packers_                               |
| 37  | `l04c3`    | _The Carrot Factory_; _Expedition Room_; level end                   |
| 38  | `l04d1`    | _The Carrot Factory_; _Warehouse 1_                                  |
| 39  | `l04d2`    | _The Carrot Factory_; _Warehouse 2_                                  |
| 40  | `l04e`     | _Objects in the mirror are closer than they appear!_; level start    |
| 41  | `l04e2`    | _Objects in the mirror are closer than they appear!_; level end      |
| 42  | `l05a1`    | _The Planet X File!_; _Space Base, AREA 1_; level start              |
| 43  | `l05a2`    | _The Planet X File!_; _Space Base, AREA 4_                           |
| 44  | `l05a3a`   | _Train your Brain!_; _Neuronal Synaptal Network_; level start        |
| 45  | `l05a3b`   | _Train your Brain!_; _Hare Dance_                                    |
| 46  | `l05a3c`   | _Train your Brain!_; _ACME Mind_                                     |
| 47  | `l05a4`    | _Train your Brain!_; bike race; level end                            |
| 48  | `l05a5`    | _The Planet X File!_; _Space Modulator_; level end                   |
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

## Level Variants

Some levels have variants indicated by a `_<N>` in the filename. For N=0-5, this
corresponds to a language ID, and is used for loading screens. N=8 is currently
unknown, and is only used for a hardcoded list of level IDs (7, 8, 21-25, 34,
and 54).

## Level Format

The BZE file format is detailed [here](bze.md).

# Function Addresses

More details about (known) functions can be found [here](functions.md).

# PlayStation Input Emulation

Internally, the game reads input from keyboard and/or joystick and maps it to a
standard PSX gamepad report format, thus emulating a PSX controller.

The rest of the information in this section can be found in the official Psy-Q
SDK documentation, but is replicated here for convenience.

The first byte of the report is always 0 to indicate success, and the second
byte is split into two 4-bit fields; the upper 4 bits indicate the "terminal
type" (aka the type of "controller"), and the lower 4 bits indicate the number
of 2-byte words of input data (not counting the two-byte "header"). All digital
button inputs are inverted, i.e. 0 = pressed, 1 = released. All axis inputs are
0-255, with 128 representing the neutral position.

Keyboard input is mapped to a 16-button controller (terminal type 4) with 2
additional bytes of input:

| offset | meaning      |
|--------|--------------|
| 2      | buttons 8-15 |
| 3      | buttons 0-7  |

Joystick input is mapped to an analog controller (terminal type 7) with 6
additional bytes of input:

| offset | meaning      |
|--------|--------------|
| 2      | buttons 8-15 |
| 3      | buttons 0-7  |
| 4      | right axis X |
| 5      | right axis Y |
| 6      | left axis X  |
| 7      | left axis Y  |

Unfortunately, the PC port only supports a single X/Y joystick input; the values
read are copied to both axes in the report.

Buttons are mapped in increasing index from right to left; for example, button
11 (the start button) is offset 2, bit 3.

Button map:

| index | button   |
|-------|----------|
| 0     | L2       |
| 1     | R2       |
| 2     | L1       |
| 3     | R1       |
| 4     | triangle |
| 5     | circle   |
| 6     | cross/X  |
| 7     | square   |
| 8     | select   |
| 9     | L3       |
| 10    | R3       |
| 11    | start    |
| 12    | up       |
| 13    | right    |
| 14    | down     |
| 15    | left     |

# Game Saves

PC game saves are split across multiple files stored in the `bin` directory:
`Gamedata.dat` and `Gamesave<N>.dat`, where N is the slot number from 1 to 6.

For comparison, the PSX version stores everything in a single memory card block
with the following frame layout (each frame is 128 bytes):

| frame | usage                    |
|-------|--------------------------|
| 0-3   | PSX standard title/icon  |
| 4-7   | main data (Gamedata.dat) |
| 8-11  | slot 1 (Gamesave1.dat)   |
| 12-15 | slot 2 (Gamesave2.dat)   |
| 16-19 | slot 3 (Gamesave3.dat)   |
| 20-23 | slot 4 (Gamesave4.dat)   |
| 24-27 | slot 5 (Gamesave5.dat)   |
| 28-31 | slot 6 (Gamesave6.dat)   |
| 32-63 | unused                   |

Each item is less than 512 bytes (aka 4 full frames); for PSX, the remaining
bytes are undefined/garbage on write, and ignored on read.

## Gamedata.dat

Main save data; 360 bytes. Most of it is unknown/unused.

| offset | type | usage        | notes                      |
|--------|------|--------------|----------------------------|
| 0x000  | int  | checksum     | sum of signed bytes        |
| 0x004  | int  | active slots | bit N = slot (N + 1) valid |
| 0x008  | SLOT | slot 1       |                            |
| 0x034  | SLOT | slot 2       |                            |
| 0x060  | SLOT | slot 3       |                            |
| 0x08c  | SLOT | slot 4       |                            |
| 0x0b8  | SLOT | slot 5       |                            |
| 0x0e4  | SLOT | slot 6       |                            |

The 44-byte SLOT sub-structure contains a summary of the save slot:

| offset | type  | usage               |
|--------|-------|---------------------|
| 0x00   | short | golden carrot count |
| 0x02   | short | clock count         |

## GamesaveN.dat

Save data for each slot; 320 bytes. For the PSX version, the active data exists
in memory at the start of usable RAM (aka 0x80010000); thus the save slot is
basically a raw dump of the first 320 bytes of this region.

| offset | type      | usage               |
|--------|-----------|---------------------|
| 0x00   | int       | current level       |
| 0x04   | int       | checksum            |
| 0x08   | short     |                     |
| 0x0a   | short     |                     |
| 0x0c   | short     |                     |
| 0x0e   | short     |                     |
| 0x10   | short     |                     |
| 0x12   | short     |                     |
| 0x14   | short     |                     |
| 0x16   | short     |                     |
| 0x18   | short     |                     |
| 0x1a   | short     |                     |
| 0x1c   | short     |                     |
| 0x1e   | short     |                     |
| 0x20   | short     |                     |
| 0x22   | short     |                     |
| 0x24   | short     |                     |
| 0x26   | short     |                     |
| 0x28   | short     |                     |
| 0x2a   | short     |                     |
| 0x2c   | short     |                     |
| 0x2e   | short     |                     |
| 0x30   | short     |                     |
| 0x32   | short     |                     |
| 0x34   | short     |                     |
| 0x36   | short     |                     |
| 0x38   | short     |                     |
| 0x3a   | short     |                     |
| 0x3c   | short     |                     |
| 0x3e   | short     |                     |
| 0x40   | byte[256] | level logic globals |

The following table lists known offsets/values in the level logic globals:

| offset | usage                         |
|--------|-------------------------------|
| 0x00   |                               |
| 0x01   | current health (half-carrots) |
| 0x02   | max health (usually 6)        |
| 0x03   | total carrot count            |
| 0x04   |                               |
| 0x05   | total clock count             |
| 0x07   | total GC count bits 0-7       |
| 0x08   |                               |
| 0x09   |                               |
| 0x0a   |                               |
| 0x0b   |                               |
| 0x0c   |                               |
| 0x0e   |                               |
| 0x0f   |                               |
| 0x10   |                               |
| 0x11   |                               |
| 0x25   | current world                 |
| 0x2d   |                               |
| 0x30   |                               |
| 0x31   |                               |
| 0x32   |                               |
| 0x33   |                               |
| 0x36   |                               |
| 0x8d   |                               |
| 0x90   |                               |
| 0x9f   | slot (1-based)                |
| 0xa5   |                               |
| 0xa6   |                               |
| 0xa7   |                               |
| 0xa8   |                               |
| 0xb2   | merlin.bze GC bitmask         |
| 0xb3   | merlin.bze GC bitmask         |
| 0xb5   | merlin.bze GC count           |
| 0xb6   |                               |
| 0xb7   |                               |
| 0xb8   |                               |
| 0xb9   |                               |
| 0xba   |                               |
| 0xc0   | merlin.bze clock bitmask      |
| 0xc1   | merlin.bze clock count        |
| 0xfb   |                               |
| 0xfc   | total GC count bits 8-15      |

Values for `current world`:

- 1 - _The Stone Age_
- 2 - _The Medieval Period_
- 3 - _The Pirate Years_
- 4 - _The 1930s_
- 5 - _Dimension X_
- 6 - _Nowhere_
