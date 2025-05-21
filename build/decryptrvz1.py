# Relatively straight-forward script to decode RVZ1's encrypted lumps in SRB2 Final Demo
# Designed to use its SECTORS and VERTEXES lumps as input
# Made by save_as

import struct
import sys

INT16_MIN = -32768
INT16_MAX = 32767

if len(sys.argv) != 3:
    print("Usage: python decryptrvz1.py input_file output_file")
    sys.exit(1)

choice = int(input("Choose lump type: \n 1. VERTEXES \n 2. SECTORS\n"))

with open(sys.argv[1], 'rb') as infile, open(sys.argv[2], 'wb') as outfile:
    offset = 0
    while True:
        int32_bytes = infile.read(4)
        if not int32_bytes:
            break

        struct_offset = offset % 36

        if choice == 2 and 8 <= struct_offset < 24:
            outfile.write(int32_bytes)
        else:
            # unpack signed 32-bit int and clamp to 16-bit range
            (int32_val,) = struct.unpack('<i', int32_bytes)
            int16_val = max(INT16_MIN, min(INT16_MAX, int32_val))

            # pack and write as signed 16-bit int
            int16_bytes = struct.pack('<h', int16_val)
            outfile.write(int16_bytes)

        offset += 4
