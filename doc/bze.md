
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

BBLiT uses a custom LZSS implementation for compression; repeated runs of bytes
are converted to small offset/length pairs, and lengths are further compressed
using a simple lookup table.

## Compression Header

The first four bytes form a small header that encode compression options and the
number of compression "items". The first byte encodes the offset size for
offset/length pairs as well as the step size for the length lookup table (LUT):

    |7 5|4  3|2   0|
    |---|step|osize|

    offset_size = 9 + osize;
    length_size = 7 - osize;

Note that an `osize` of 7 is not valid (since no lengths could be encoded).

The remaining 3 bytes form a 24-bit big-endian integer that represents the
number of compression "items"; the stored value is one less than the actual
value (so there is always at least one "item").

## Length Encoding

Length values are further compressed via a LUT that can be pre-calculated from
the first header byte. The entries start at 3[^lut] (the "break-even" point) and
increment by 1 until they hit a threshold, at which point they increment by a
larger step.

The size of the LUT is given by the size of the length field; this is then used
to calculate the threshold:

    lut_size = 1 << length_size;
    if (lut_size >= 32)
        lut_thres = 19;
    else
        lut_thres = lut_size / 2 - 1;

Entry `i` in the LUT is then computed as follows:

    if (i > lut_thres)
        lut[i] = 3 + ((i - lut_thres) << step) + lut_thres;
    else
        lut[i] = 3 + i;

[^lut]: The game actually builds the LUT with entries starting at 2, but uses
them to represent 0-based counts; the given description/code is equivalent.

## Compression Body

The remainder of the compressed section is a sequence of compression "items".
Items are grouped by 8 and prefixed with a byte that encodes their types; bit 0
(lsb) corresponds to the first item. If the number of items is not divisible by
8, the unused bits in the last group prefix byte are 0.

Item type 0 represents an offset/length pair. The next two bytes form a 16-bit
big-endian integer which encode the offset and compressed length according to
the bit sizes calculated from the header; the offset field resides in the upper
bits of the 16-bit integer. The compressed length field is used as an index into
the LUT to get the actual length.

The resulting offset/length pair means "copy 'length' bytes starting 'offset'
bytes backward from the end of the current decompressed buffer". In some cases,
the length may be greater than the offset, meaning that some/all of the bytes
are copied multiple times. For example, given an offset/length pair of 3/8, and
assuming the last 4 decompressed bytes are `de ad be ef`, then the additional
decompressed bytes would be `ad be ef ad be ef ad de`.

Example C code (`src` points to current item; `dst` points to destination):

    tmp = (src[0] << 8) | src[1];
    src += 2;
    offset = (tmp >> length_size) & ((1 << offset_size) - 1);
    length_enc = tmp & ((1 << length_size) - 1);
    length = lut[length_enc];
    const BYTE *dst_hist = dst - offset;
    for (i = 0; i < length; i++)
        *dst++ = *dst_hist++;

Item type 1 represents a literal byte, which is simply copied from input to
output.

# Section 1

This section consists of a sequence of tags/"instructions" that tell the game
how to load assets and build up the world and entities inside it.

Most of the section consists of "chunks" that start with a `2d` byte and end
with a `2e` byte; these chunks are described in the next paragraph. Additional
"top-level" meaningful bytes include `2c` and `4b`, which have unknown meanings,
and `2f`, which signals the end of the section. Finally, if the section starts
with `2d 45`, the game reads a pseudo-chunk before main scanning.

Each chunk starts with a `2d` byte, a "type" byte, zero or more "attribute" or
"modifier" tags, and a terminating `2e` byte. Each tag is an identifier byte
followed by a fixed number of bytes, depending on the identifier. The following
sub-sections list known details about chunks, valid tags, and the number of
extra bytes for each tag.

Some notes about data types:
* position/scale coordinates are signed fixed-point longs (20.12)
* angles are signed fixed-point shorts (4.12), where 0x1000 means 360 degrees

## Type 0x00

| tag  | length |
|------|--------|
| 0x01 | 8      |
| 0x02 | 8      |
| 0x04 | 8      |
| 0x44 | 12     |
| 0x4c | 48     |
| 0x4d | 48     |

## Type 0x05

| tag  | length |
|------|--------|
| 0x06 | 3      |

## Type 0x07/0x08/0x0a

This chunk seems to indicate an entity/object.

| tag  | length |
|------|--------|
| 0x0b | 4      |
| 0x0c | 4      |
| 0x0f | 2      |
| 0x10 | 12     |
| 0x11 | 6      |
| 0x12 | 12     |
| 0x13 | 1      |
| 0x15 | 1      |
| 0x16 | 8      |
| 0x17 | 36     |
| 0x18 | 16     |
| 0x19 | 4      |
| 0x1a | 4      |
| 0x1b | 6      |
| 0x1c | 8      |
| 0x1d | 4      |
| 0x1e | 2      |
| 0x1f | 1      |
| 0x27 | 4      |
| 0x30 | 32     |
| 0x31 | 32     |
| 0x32 | 12     |
| 0x34 | 24     |
| 0x35 | 40     |
| 0x38 | 2      |
| 0x39 | 2      |
| 0x3a | 4      |
| 0x41 | 20     |
| 0x42 | 6      |
| 0x46 | 2      |

### Tag 0x10

Initial entity position.

| offset | type | usage |
|--------|------|-------|
| 0      | long | X     |
| 4      | long | Y     |
| 8      | long | Z     |

### Tag 0x11

Initial entity rotation.

| offset | type  | usage |
|--------|-------|-------|
| 0      | short | angle |
| 2      | short | angle |
| 4      | short | angle |

### Tag 0x12

Initial entity scale.

| offset | type | usage |
|--------|------|-------|
| 0      | long | X     |
| 4      | long | Y     |
| 8      | long | Z     |

### Tag 0x13

This tag consists of a single byte. All that's known is that if this byte is
`01`, the chunk corresponds to the player.

### Tag 0x27

This tag includes the entity ID.

| offset | type  | usage     |
|--------|-------|-----------|
| 0      | short | entity ID |
| 2      | short |           |

### Tag 0x31

This tag is related to BZE logic. Condition/action codes and args are detailed
in the "Level Logic" section.

| offset | type  | usage           |
|--------|-------|-----------------|
| 12     | byte  | condition code  |
| 13     | byte  | condition arg 1 |
| 14     | byte  | condition arg 2 |
| 15     | byte  | action code     |
| 16     | short | action arg 1    |
| 18     | short | action arg 2    |

## Type 0x09

| tag  | length |
|------|--------|
| 0x10 | 12     |
| 0x11 | 6      |
| 0x16 | 8      |
| 0x1c | 8      |
| 0x1f | 1      |
| 0x33 | 32     |

## Type 0x20

| tag  | length |
|------|--------|
| 0x10 | 12     |
| 0x11 | 6      |
| 0x12 | 12     |
| 0x21 | 4      |
| 0x24 | 8      |
| 0x39 | 2      |
| 0x43 | 1      |

## Type 0x22

This chunk may be used to load an asset.

| tag  | length |
|------|--------|
| 0x0d | 12     |
| 0x24 | 8      |
| 0x25 | 8      |
| 0x26 | 4      |
| 0x27 | 4      |
| 0x28 | 3      |
| 0x3f | 12     |
| 0x40 | 8      |

### Tag 0x25

| offset | type | usage       |
|--------|------|-------------|
| 0      | int  | offset      |
| 4      | int  | header size |

### Tag 0x27

This tag includes the asset ID, which is how entities reference them.

| offset | type  | usage    |
|--------|-------|----------|
| 2      | short | asset ID |

### Tag 0x40

| offset | type | usage       |
|--------|------|-------------|
| 0      | int  | offset      |
| 4      | int  | header size |

## Type 0x29

| tag  | length |
|------|--------|
| 0x2a | 8      |

## Type 0x36

| tag  | length |
|------|--------|
| 0x37 | 8      |

## Type 0x3b

| tag  | length |
|------|--------|
| 0x2b | 12     |
| 0x3c | 4      |
| 0x3d | 6      |
| 0x3e | 28     |

### Tag 0x2b

This seems to set the initial camera position.

| offset | type | usage |
|--------|------|-------|
| 0      | long | X     |
| 4      | long | Y     |
| 8      | long | Z     |

## Type 0x47

| tag  | length |
|------|--------|
| 0x48 | 8      |

## Type 0x49

| tag  | length |
|------|--------|
| 0x4a | 48     |

# Level Logic

Game logic in each level is built out of logic "items", each of which consists
of a condition and an action; each condition/action has up to two arguments,
8/16 bits each, respectively. More details about condition/action codes can be
found in the decompiled source in `logic_prv.h`.

The following table summarizes known condition codes (codes in *italics* are
specific to the PC port):

| code | mnemonic | meaning                                       |
|------|----------|-----------------------------------------------|
| 0x00 | true     | always true                                   |
| 0x01 | ciseq    | compare immed/scratch[^scratch] equal         |
| 0x02 | cigeq    | compare immed/global[^global] equal           |
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
| 0x60 | *tkdesc* | test key down: escape                         |

The following table summarizes known action codes:

| code | mnemonic | meaning                   |
|------|----------|---------------------------|
| 0x00 | nop      | no operation              |
| 0x03 | incs     | increment scratch         |
| 0x04 | incg     | increment global[^gmod]   |
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

[^scratch]: `scratch` refers to a 256-byte array of temporary data
[^global]: `global` refers to the 256-byte array at the end of GamesaveN.dat
[^gmod]: most operations that modify globals have code to handle the total
golden carrot count, which is split across two bytes
