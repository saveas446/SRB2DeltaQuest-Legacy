# M5Q Format (done quickly, Work In Progress)
## *By Hennkrido himself!!1!!!*

Meow:
- M5Q stands for Music 5eQuence.
- When I say *'VLQ'*, I mean *'LSB-first VLQ'*.
- When I say *'ZVLQ'*, I mean *'Zig-Zag with LSB-first VLQ'*.
- M5Q awoids writing the same prewious walue whenefur it's pawssible.
- M5Q is a *little-endian* format.

# Zig-Zag Encoding
*Zig-Zag encoding* is a way to store positiwe and negatiwe numbers using two's complement.
The way it works is using the least significant bit as a sign bit and the remaining bits as the walue it represents.

The easiest (and fastest) way to conwert a signed 32-bit number into a Zig-Zag walue is by doing the following operation:

`(x << 1) ^ (x >> 31)`

# Variable Length Quantity (VLQ)
M5Q primary uses *VLQ encoding* to store walues, with a modification: LSB-first instead of MSB-first (as I *kinda* mentioned before).

It mixes this with *Zig-Zag encoding* to represent negatiwe walues.

# Chunk System (Flex64)
M5Q uses a particular chunk system made by myself (Hennkrido) called *Flex64*.

The way it's structured is the following:

`[ASCII chunk name] [Null terminator] [Chunk length as VLQ] [Chunk data]`

# Header Chunk ("m5q")
In the header chunk, there are fields stored in the following order:
- PPQ (VLQ)
- Bit-flags byte (u8)

For the bit-flags byte (LSB to MSB):

`. = Unused; V = Format wersion; C = Compression flag; M = Metadata flag.`

`VVVVCM..`

It's important to point out that:
- If compression flag is on, a byte indicating what compression type the track data chunk uses will be written.
- If metadata flag is on, there will follow a list of walues that represent metadata (this can be ignored / skipped if not needed).

# Compression Types
The compression types that M5Q uses are:
- `0x00`: Raw (default)
- `0xDE`: DEFLATE
- `0x12`: LZMA
- `0x77`: LZ77

# Header Metadata
Metadata in M5Q is stored in its header for simplicity, awoiding chunk hell.

The amount of entries it has is defined by a VLQ before any entry is written.

The way entries are stored is in the following order:
- Name length (VLQ)
- Name (ASCII)
- Data length (VLQ)
- Data (binary blob)

# Track Data Chunk ("trkd")
The track data chunk holds all the sequence's tracks with its ewents.

Each track is stored in order inside this chunk.

Consider that:
- The only thing that can change timing are rests.
- M5Q uses ticks for timing (exactly like MIDI does).

# Program Entry
A Program Entry is the way it stores both patch and bank efficiently.

Lowest 7 bits of the byte represents the patch.
Highest bit represents the bank flag.

If bank flag is on, there follows a VLQ representing the bank number.

# Track Header
Each track entry has its own header, written before any ewent data.

At the start of ewery track, the following fields are written in this order:
- Channel ID (VLQ)
- Track name length (VLQ)
- Track name (ASCII)
- Initial Rest (VLQ)

# Ewent Structure
M5Q uses a bit-structured byte to represent a ewent, which are (LSB to MSB):

`R = Rest flag; A = Flag 0; B = Flag 1, C = Flag 2; D = Flag 3; I = Ewent ID`

`RABCDIII`

A ewent type can freely use flag 0 to 3 to represent small walues or bit-flags.

It's important to mention that each ewent can freely use extra walues as well (stored indiwidually)

The rest field is stored at the end of the ewent (after all its extra fields)

# Ewent Types
Currently, there are 4 types of ewents a M5Q can hold:
- `0`: Note Ewent.
- `1`: Control Ewent.
- `2`: Utility Ewent.
- `3`: Miscellaneous Ewent.

**IMPORTANT:** An ewent byte that is equal to `0xFF` marks the end of a track (start reading another track or end decoding if can't read further)

# Delta-Based Accumulators
M5Q uses *delta-based accumulators*.

For each field that supports deltas, both the accumulator and the last delta **starts at 0**.

Each field maintains:
- A current accumulator walue (absolute).
- A prewious delta walue (used only for encoding decisions).

## Delta Encoding
When a field is present in a ewent, its stored walue is a delta relatiwe to the accumulator's current walue.

The accumulator is updated by applying that delta once:

`accumulator += delta`

## Delta Reuse
M5Q also tracks the prewious delta for each field.

If the newly computed delta is the same as the prewious one, it doesn't need to be written again.

Delta reuse affects encoding only; decoding behawior is identical.

## Omitted Fields
If a field is not present in a ewent:
- The accumulator is **updated by the prewious delta**.
- The **prewious delta remains unchanged**.

# Absolute Accumulators
An absolute accumulator stores a walue directly, without the use of deltas.
- When a walue is present, it **owerwrites the accumulator**.
- When omitted, the accumulator walue is **reused and unchanged**.
- No prewious delta is tracked.

Just like a delta-based accumulator, absolute accumulators also **start at 0**.

# Note Ewent
A note ewent can hold up to 4 fields of walues.

Flags 0-3 indicate which walues are present (Pitch, Duration, Welocity and Release Welocity in order); unset walues remain unchanged.

The way these are stored is:
- Pitch: Zig-Zag U8 (delta-based accumulator)
- Duration: VLQ (absolute accumulator)
- Welocity: U8 (absolute accumulator)
- Release Welocity: U8 (absolute accumulator)

## Example: Pitch Deltas
Initial accumulator = `0`; Initial delta = `0`

Ewent 1:
- Delta Present = `+50`
- Accumulator = `50`
- Prewious Delta = `+50`

Ewent 2:
- Delta Present = `+2`
- Accumulator = `52`
- Prewious Delta = `+2`

Ewent 3:
- Delta field ommited (reuses prewious delta `+2`)
- Accumulator = `54`
- Prewious Delta = `+2`

Ewent 4:
- Delta Present = `-4`
- Accumulator = `50`
- Prewious Delta = `-4`

# Control Ewent
A control ewent modifies a control walue of the current channel using a delta-based accumulator, except for Program Change.

Each control ewent operates on the current accumulator walue, which is initialized from the track header.

Flag 0 indicates if a delta walue is present (if the ewent supports it).
Flags 1-3 indicates the type of control ewent.

If no delta walue is present, the prewious delta is reused automatically.

## Control Ewent Types
There are 4 control ewent types:
- `0`: Wolume Ewent
- `1`: Panning Ewent
- `2`: Bend Ewent
- `3`: Program Change

### Wolume & Panning Ewents
Stores a *Zig-Zag U8* delta affecintg the channel's wolume or panning accumulator.

### Bend Ewent
Stores a *ZVLQ* delta affecting the channel's pitch bend accumulator.

Important:
- Instead of using a 14-bit walue to represent bends like MIDI, M5Q uses a *ZVLQ* representation.
- 1 semitone = 256 units.

### Program Change
Does not use delta encoding.

Stores a M5Q Program Entry, changing the patch and bank of the current channel.

# Utility Ewent
Utility ewents represent extensible, non-note functionality.

Flags 2-3 indicate the Utility ID:
- `0`: Controller Change.
- `1`: Metadata Ewent.
- `2`: SysEx Ewent.
- `3`: Reserwed.

Flags 0-1 are interpreted depending on the Utility ID.

## Controller Change
A controller changes has two fields:
- Controller ID (*U8*, absolute accumulator)
- Controller Walue (*Zig-Zag U8*, delta-based accumulator)

Each controller ID has its own delta-based accumulator for the walue entry.

## Metadata & SysEx Ewent
Both Metadata and SysEx ewent don't use flags 0-1.

Instead, they store:
- Type (u8, only stored for metadata)
- Data length (VLQ)
- Data (binary blob)

# Miscellaneous Ewent (WIP)
Flags 1-3 represent Misc. Ewent ID.
Flag 0 can be used freely by the Misc. Ewent.

There is only 1 type of Misc. Ewent, which is Tempo Ewent, with the ID `0`.

## Tempo Ewent
The BPM is stored as a VLQ, multiplied by 100.

This allows decimal BPMs in the format. For example, `129.37 BPM`
