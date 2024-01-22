
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

| offset | type | usage        |
|--------|------|--------------|
| 0x0    | int  | section ID   |
| 0x4    | int  | actual size? |
| 0x8    | int  | file size    |

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
8/16 bits each, respectively.

The following table lists known condition codes, with arguments A and B (all
comparisons are unsigned unless otherwise specified):

| code | condition                   |
|------|-----------------------------|
| 0x00 | always `true`               |
| 0x01 | `A == scratch[B]`[^1]       |
| 0x02 | `A == globals[B]`[^2]       |
| 0x03 | `A < scratch[B]`            |
| 0x04 | `A < globals[B]`            |
| 0x05 | `A <= scratch[B]`           |
| 0x06 | `A <= globals[B]`           |
| 0x07 | `A > scratch[B]`            |
| 0x08 | `A > globals[B]`            |
| 0x09 | `A >= scratch[B]`           |
| 0x0a | `A >= globals[B]`           |
| 0x0f | `scratch[A] == scratch[B]`  |
| 0x10 | `globals[A] == globals[B]`  |
| 0x1e | `(A & scratch[B]) != 0`     |
| 0x1f | `(A & globals[B]) != 0`     |
| 0x22 | `(A & scratch[B]) == 0`     |
| 0x23 | `(A & globals[B]) == 0`     |
| 0x25 | `A != scratch[B]`           |
| 0x26 | `A != globals[B]`           |
| 0x27 | same as code 0x22           |
| 0x28 | same as code 0x23           |
| 0x2e | same as code 0x01           |
| 0x2f | same as code 0x02           |
| 0x31 | action key pressed          |
| 0x33 | player is stationary        |
| 0x35 | player is moving            |
| 0x3d | same as code 0x0f           |
| 0x3e | same as code 0x10           |
| 0x3f | `scratch[A] != scratch[B]`  |
| 0x40 | `globals[A] != globals[B]`  |
| 0x41 | `scratch[A] < scratch[B]`   |
| 0x42 | `globals[A] < globals[B]`   |
| 0x43 | `scratch[A] > scratch[B]`   |
| 0x44 | `globals[A] > globals[B]`   |
| 0x46 | signed version of code 0x03 |
| 0x47 | signed version of code 0x07 |
| 0x48 | signed version of code 0x41 |
| 0x49 | signed version of code 0x43 |
| 0x51 | action key released         |
| 0x58 | signed version of code 0x04 |
| 0x59 | signed version of code 0x08 |

The following table lists known action codes, with arguments X and Y:

| code | action                           |
|------|----------------------------------|
| 0x00 | do nothing                       |
| 0x04 | `globals[Y]++`[^3]               |     
| 0x0c | `globals[Y] |= X`                |
| 0x17 | `globals[Y]++, globals[X]++`[^3] |
| 0x30 | give carrot                      |

[^1]: `scratch` refers to a small array of temporary data
[^2]: `globals` refers to the 256-byte array at the end of GamesaveN.dat
[^3]: the game has special code to handle the total golden carrot count, which
is split across two bytes
