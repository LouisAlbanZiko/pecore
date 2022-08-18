#include "cc_internal.h"

CC_StringBuffer *CC_StringBuffer_Create(uint64_t start_length)
{
	CC_StringBuffer *buffer = CC_Malloc(sizeof(*buffer));
	
	buffer->countCurrent = 0;
	buffer->countMemory = (start_length == 0) * (uint64_t)64 + start_length;
	buffer->data = CC_Malloc(sizeof(*buffer->data) * buffer->countMemory);

	return buffer;
}

void CC_StringBuffer_Destroy(CC_StringBuffer *buffer)
{
	CC_Free(buffer->data);
	CC_Free(buffer);
}

void CC_StringBuffer_InsertChar(CC_StringBuffer *buffer, char c)
{
	if (buffer->countCurrent == buffer->countMemory)
	{
		buffer->countMemory <<= 1;
		buffer->data = CC_Realloc(buffer->data, sizeof(*buffer->data) * buffer->countMemory);
	}
	buffer->data[buffer->countCurrent] = c;
	buffer->countCurrent++;
}

void CC_StringBuffer_InsertString(CC_StringBuffer *buffer, const char *string)
{
	uint64_t length = strlen(string);
	if (buffer->countCurrent + length >= buffer->countMemory)
	{
		buffer->countMemory <<= 1;
		buffer->data = CC_Realloc(buffer->data, sizeof(*buffer->data) * buffer->countMemory);
	}
	for (uint64_t i = 0; i < length; i++)
	{
		buffer->data[buffer->countCurrent] = string[i];
		buffer->countCurrent++;
	}
}

void CC_StringBuffer_InsertSubString(CC_StringBuffer *buffer, const char *string, uint64_t length)
{
	if (buffer->countCurrent + length >= buffer->countMemory)
	{
		buffer->countMemory <<= 1;
		buffer->data = CC_Realloc(buffer->data, sizeof(*buffer->data) * buffer->countMemory);
	}
	for (uint64_t i = 0; i < length; i++)
	{
		buffer->data[buffer->countCurrent] = string[i];
		buffer->countCurrent++;
	}
}

char *CC_StringBuffer_ToStringAndDestroy(CC_StringBuffer *buffer, uint64_t *length)
{
	char *string = buffer->data;
	if(length != NULL)
	{
		*length = buffer->countCurrent;
	}
	CC_StringBuffer_InsertChar(buffer, '\0');
	CC_Free(buffer);
	return string;
}
