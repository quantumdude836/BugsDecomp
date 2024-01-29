
The BZE file format is used for all BBLiT levels. It consists of several
sections, some of which are compressed using a custom LZ-like algorithm.

# Header

| offset | type | usage           |
|--------|------|-----------------|
| 0x000  | int  | version?        |
| 0x004  | int  | section count   |
| 0x008  | *    | sections        |
| 0x7fc  | int  | header checksum |

`sections` refers to an array of section header entries; `section count` gives
the number of entries.

The checksum is simply the 32-bit sum of bytes 0x000-0x7fb, treated as signed
8-bit integers.

Each entry has the following structure:

| offset | type | usage       |
|--------|------|-------------|
| 0x0    | int  | section ID  |
| 0x4    | int  | actual size |
| 0x8    | int  | file size   |

Sections are stored consectively in the BZE in the order given by the header.
Each section occupies `file size` bytes in the BZE, of which `actual size` bytes
are valid (the rest are padding).

Known section IDs:

| ID | compressed? | usage               |
|----|-------------|---------------------|
| 1  | yes         | load "instructions" |

# Compression Algorithm

BBLiT uses a custom LZ-like algorithm for compressing certain sections, allowing
repeated runs of bytes to be efficiently encoded.

The first byte in a compressed blob encodes a `shift` and `step` field:

    |7 5|4  3|2   0|
    |---|step|shift|

These fields are used to compute the following values:

    lut_size = 128 >> shift;
    lut_thres = (lut_size >= 32) ? 19 : (lut_size >> 1) - 1;

These values are then used to build a LUT (look-up table) of size `lut_size`.
Entry `i` is computed as follows:

    if (i > lut_thres)
        lut[i] = ((i - lut_thres) << step) + lut_thres + 2;
    else
        lut[i] = i + 2;

This LUT allows for longer runs to be encoded than what could normally be stored
in a small integer.

The next 3 bytes form a big-endian 24-bit integer encoding the number of "items"
that follow; the value stored is 1 less than the actual count. Items are grouped
by 8; each group is prefixed with a byte indicating each item type, where the
lowest bit corresponds to the first group.

If the item type is 0, the next two bytes form a big-endian 16-bit integer,
where `N = 7 - shift`:

    |15   N|N-1     0|
    |offset|lut_index|

`lut_index` encodes a size according to the LUT:

    size = lut[lut_index];

Together `offset` and `size` refer to a run of previously-decompressed bytes to
copy; specifically, `offset` points backwards from the current output position.
Note that `size` can be larger than `offset`, meaning that the previous run of
bytes is effectively duplicated. For example, given an `offset/size` pair of
`3/8`, and assuming the last 3 decompressed bytes are `24 68 ac`, then the
additional decompressed bytes would be `24 68 ac 24 68 ac 24 68`.

If the item type is 1, then the next byte is copied straight to output.

# Section 1

This section consists of a sequence of tags/"instructions" that tell the game
how to load assets and build up the world and entities inside it.

# Level Logic

Game logic in each level is built out of logic "items", each of which consists
of a condition and an action; each condition/action has up to two arguments,
8/16 bits each, respectively. More details about condition/action codes can be
found in the decompiled source in `logic_prv.h`.

The following table summarizes known condition codes:

| code | mnemonic | meaning                                       |
|------|----------|-----------------------------------------------|
| 0x00 | true     | always true                                   |
| 0x01 | ciseq    | compare immed/scratch equal[^1]               |
| 0x02 | cigeq    | compare immed/global equal[^2]                |
| 0x03 | cisltu   | compare immed/scratch less than, unsigned     |
| 0x04 | cigltu   | compare immed/global less than, unsigned      |
| 0x05 | cisleu   | compare immed/scratch less/equal, unsigned    |
| 0x06 | cigleu   | compare immed/global less/equal, unsigned     |
| 0x07 | cisgtu   | compare immed/scratch greater than, unsigned  |
| 0x08 | ciggtu   | compare immed/global greater than, unsigned   |
| 0x09 | cisgeu   | compare immed/scratch greater/equal, unsigned |
| 0x0a | ciggeu   | compare immed/global greater/equal, unsigned  |
| 0x0f | csseq    | compare two scratches equal                   |
| 0x10 | cggeq    | compare two globals equal                     |
| 0x1e | tismnz   | test immed/scratch masked non-zero            |
| 0x1f | tigmnz   | test immed/global masked non-zero             |
| 0x22 | tismz    | test immed/scratch masked zero                |
| 0x23 | tigmz    | test immed/global masked zero                 |
| 0x25 | cisne    | compare immed/scratch not equal               |
| 0x26 | cigne    | compare immed/global not equal                |
| 0x27 | tisaz    | test immed/scratch bitwise-and zero           |
| 0x28 | tigaz    | test immed/global bitwise-and zero            |
| 0x2e | tisxz    | test immed/scratch bitwise-xor zero           |
| 0x2f | tigxz    | test immed/global bitwise-xor zero            |
| 0x31 | tbdact   | test button down: action                      |
| 0x33 | tpnmv    | test player not moving                        |
| 0x35 | tpmov    | test player moving                            |
| 0x3d | cseq     | compare scratches equal                       |
| 0x3e | cgeq     | compare globals equal                         |
| 0x3f | csne     | compare scratches not equal                   |
| 0x40 | cgne     | compare globals not equal                     |
| 0x41 | csltu    | compare scratches less than, unsigned         |
| 0x42 | cgltu    | compare globals less than, unsigned           |
| 0x43 | csgtu    | compare scratches greater than, unsigned      |
| 0x44 | cggtu    | compare globals greater than, unsigned        |
| 0x46 | cislt    | compare immed/scratch less than, signed       |
| 0x47 | cisgt    | compare immed/scratch greater than, signed    |
| 0x48 | cslt     | compare scratches less than, signed           |
| 0x49 | csgt     | compare scratches greater than, signed        |
| 0x51 | tbuact   | test button up: action                        |
| 0x58 | ciglt    | compare immed/global less than, signed        |
| 0x59 | ciggt    | compare immed/global greater than, signed     |
| 0x60 | tkdesc   | test key down: escape                         |

The following table summarizes known action codes:

| code | mnemonic | meaning                   |
|------|----------|---------------------------|
| 0x00 | nop      | no operation              |
| 0x03 | incs     | increment scratch         |
| 0x04 | incg     | increment global[^3]      |
| 0x05 | decs     | decrement scratch         |
| 0x06 | decg     | decrement global          |
| 0x07 | clrs     | clear scratch             |
| 0x08 | clrg     | clear global              |
| 0x09 | negs     | negate scratch            |
| 0x0a | negg     | negate global             |
| 0x0b | orsi     | or scratch with immed     |
| 0x0c | orgi     | or global with immed      |
| 0x0d | andsi    | and scratch with immed    |
| 0x0e | andgi    | and global with immed     |
| 0x0f | rnds     | randomize scratch         |
| 0x10 | rndg     | randomize global          |
| 0x12 | movsi    | move scratch from immed   |
| 0x13 | movgi    | move global from immed    |
| 0x14 | movss    | move scratch from scratch |
| 0x15 | movgg    | move global from global   |
| 0x16 | incss    | increment 2 scratches     |
| 0x17 | incgg    | increment 2 globals       |
| 0x1b | movgs    | move global from scratch  |
| 0x1c | movsg    | move scratch from global  |
| 0x24 | addsi    | add scratch with immed    |
| 0x25 | addgi    | add global with immed     |
| 0x27 | subsi    | sub scratch with immed    |
| 0x28 | subgi    | sub global with immed     |
| 0x30 | gvcrt    | give carrot               |

[^1]: `scratch` refers to a 256-byte array of temporary data
[^2]: `global` refers to the 256-byte array at the end of GamesaveN.dat
[^3]: the game has special code to handle the total golden carrot count, which
is split across two bytes
