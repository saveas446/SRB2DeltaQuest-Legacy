#ifndef _M_FILEWRITER_BE_H
#define _M_FILEWRITER_BE_H

#include "doomtype.h"

typedef struct tagFileBuffer {
	UINT8* buffer;
	size_t pos, size;
} FileBuffer;

// write helpers def
void fw_write_u8(FileBuffer* fw, UINT8 val);

void fw_write_string(FileBuffer* fw, const char* str);
void fw_write_bytes(FileBuffer* fw, const UINT8* data, size_t len);

void fw_le_write_u16(FileBuffer* fw, UINT16 val);
void fw_le_write_u32(FileBuffer* fw, UINT32 val);

void fw_be_write_u16(FileBuffer* fw, UINT16 val);
void fw_be_write_u32(FileBuffer* fw, UINT32 val);

UINT32 fw_get_crc32(FileBuffer* fw, size_t offset, size_t len);
UINT32 buf_get_crc32(const UINT8* buf, size_t len);

#endif