# M5Q Format Documentation (done quickly, Work In Progress)
## *By Hennkrido herself (fur sawe :3)*

Before reading, I need to make sure you acknowledge the following:
- M5Q stands for Music 5eQuence.
- When I deconstruct a byte by bits, it will be from least significant bit to most significant bit.
- When I say 'VLQ', I mean 'LSB-first VLQ'.
- When I say 'ZVLQ', I mean 'Zig-Zag with LSB-first VLQ'.
- M5Q awoids writing the same prewious walue whenefur it's pawssible (:3c)

# Zig-Zag Encoding
Zig-Zag encoding is a way to store positiwe and negatiwe numbers using two's complement.
The way it works is using the least significant bit as a sign bit and the remaining bits as the walue it represents.

The easiest (and fastest) way to conwert a signed 32-bit number into a Zig-Zag walue is by doing the following operation:

`(x << 1) ^ (x >> 31)`

# Variable Length Quantity (VLQ)
M5Q primary uses VLQ encoding to store walues, with a modification:LSB-first instead of MSB-first (as I *kinda* mentioned before).

It mixes this with Zig-Zag encoding to represent negatiwe walues.

# M5Q's Chunk System (Flex64)
M5Q uses a particular chunk system made by myself (Hennkrido).
The way it's structured is the following:

`[ASCII chunk name] [Null terminator] [Chunk length as VLQ] [Chunk data]`

# M5Q's Header Chunk
M5Q can store data in its header (stored as "m5q"), which are (in sequential order):
- PPQ (VLQ)
- Tempo (VLQ, multiplied by 100 for decimals)
- Bit-flags byte (u8)

Now, for the bit-flags byte:

`. = Unused; V = Format wersion; C = Compression flag; M = Metadata flag.`

`VVVVCM..`

It's important to point out that:
- If compression flag is on, there will be a byte that indicates what compression type the track data chunk uses.
- If metadata flag is on, there will follow a list of walues that represent metadata (this can be ignored / skipped if not needed).

# M5Q's Compression Types
The compression types that M5Q uses are:
- `0x00`: Raw (default)
- `0xDE`: DEFLATE
- `0x14`: LZMA
- `0x77`: LZ77

# M5Q's Header Metadata
Metadata in M5Q is stored in its header for simplicity (awoiding chunk hell).
The amout of entries it has is defined by a VLQ.

Now, the way entries are stored is the following (in sequential order):
- Name length (VLQ)
- Name (ASCII)
- Data length (VLQ)
- Data (binary blob)

# M5Q's Track Data Chunk
M5Q's track data chunk (stored as "trkd") holds all the sequence's tracks with its ewents.

Each track is stored sequentially inside this chunk.

Now, consider that:
- The only thing that can change timing are rests.
- M5Q uses ticks for timing (exactly like MIDI does).

# M5Q's "Program"
A M5Q program is the way it stores both patch and bank in the same walue.

The bits it uses are:

`P = Patch walue; B = Bank flag`

`PPPPPPPB`

Now, consider that if bank flag is on, it follows a VLQ representing the bank number.

# M5Q's Track Header
Each track entry has its own header before ewent data.

Its header is stored as (sequential order):
- Channel (VLQ)
- Track name length (VLQ)
- Track name (ASCII)
- Patch + Bank (M5Q Program)
- Initial Wolume (U8)
- Initial Panning (U8)
- Initial Bend (ZVLQ)
- Initial Rest (VLQ)

# M5Q's Ewent Structure
M5Q uses a bit-structured byte to represent a ewent, which are:
`R = Rest flag; A = Flag 0; B = Flag 1, C = Flag 2; D = Flag 3; I = Ewent ID`

`RABCDIII`

A ewent type can freely use flag 0 to 3 to represent small walues or bit-flags.

It's important to mention that each ewent can freely use extra walues as well (stored indiwidually)

# M5Q's Ewent Types
Currently, there are 4 types of ewents a M5Q can hold:
- `0b000`: Note Ewent.
- `0b100`: Control Ewent.
- `0b010`: Utility Ewent.
- `0b110`: Miscellaneous Ewent.

IMPORTANT: An ewent byte that is equal to `0xFF`, marks the end of a track.

# M5Q's Ewent Types Contents
Now, I will show you how M5Q's ewents hold MIDI data.

## Note Ewent
A note ewent can hold up to 4 fields of walues.
Flags 0-3 indicate which walues are present (Pitch, Duration, Welocity and Release Welocity in order); unset walues remain unchanged.

Now, the way these are stored is:
- Pitch: Zig-Zag U8
- Duration: VLQ
- Welocity: U8
- Release Welocity: U8

It's important to point out that Pitch is stored as a delta from the prewious walue (0 by default), which helps M5Q compress arpeggios and chords better.

## Control Ewent
A control ewent stores delta from prewious walue (default is the one defined on header), except for Program Change.