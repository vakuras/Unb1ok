#ifndef _V_OGG_MEMORY_H_
#define _V_OGG_MEMORY_H_

#include <windows.h>
#include <ogg\ogg.h>

typedef struct
{
	LPVOID			pBuffer;
	ogg_int64_t		Size;
	ogg_int64_t		Read;
	BOOL			DeleteOnClose;
} OGGBUFFER, *POGGBUFFER;

size_t oggbuffer_read(void* data_ptr, size_t byteSize, size_t sizeToRead, void* data_src);
int oggbuffer_seek(void* data_src, ogg_int64_t offset, int origin);
long oggbuffer_tell(void* data_src);
int oggbuffer_close(void* data_src);

#endif