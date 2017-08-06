#include "vOggMemory.h"

long oggbuffer_tell(void* data_src) 
{
	POGGBUFFER oggData = (POGGBUFFER)data_src;
	if (NULL == oggData) 
		return -1;
	
	return oggData->Read;
}

int oggbuffer_seek(void* data_src, ogg_int64_t offset, int origin)
{
	POGGBUFFER oggData = (POGGBUFFER)data_src;
	if (NULL == oggData) 
		return -1;

	switch (origin)
	{
		case SEEK_SET: 
		{ 
			ogg_int64_t actualOffset; 

			if (oggData->Size >= offset)
				actualOffset = offset;
			else
				actualOffset = oggData->Size;

			oggData->Read = actualOffset;
			break;
		}

		default:
		case SEEK_CUR: 
		{
			size_t spaceToEOF = oggData->Size - oggData->Read;

			ogg_int64_t actualOffset; 

			if (offset < spaceToEOF)
				actualOffset = offset;
			else
				actualOffset = spaceToEOF;  

			oggData->Read += actualOffset;
			break;
		}

		case SEEK_END: 
			oggData->Read = oggData->Size+1;
			break;
		break;
	};

	return 0;
}

size_t oggbuffer_read(void* data_ptr, size_t byteSize, size_t sizeToRead, void* data_src)
{
	POGGBUFFER oggData = (POGGBUFFER)data_src;
	if (NULL == oggData) 
		return -1;

	size_t actualSizeToRead, spaceToEOF = oggData->Size - oggData->Read;

	if ((sizeToRead*byteSize) < spaceToEOF)
		actualSizeToRead = (sizeToRead*byteSize);
	else
		actualSizeToRead = spaceToEOF;  

	if (actualSizeToRead)
	{
		memcpy(data_ptr, (PBYTE)oggData->pBuffer + oggData->Read, actualSizeToRead);
		oggData->Read += actualSizeToRead;
	}

	return actualSizeToRead;
}

int oggbuffer_close(void* data_src)
{
	POGGBUFFER oggData = (POGGBUFFER)data_src;
	if (NULL == oggData || !oggData->DeleteOnClose) 
		return 0;

	if (oggData->pBuffer != NULL)
	{
		delete[] oggData->pBuffer;
		oggData->pBuffer = NULL;
	}

	delete oggData;

	return 0;
}