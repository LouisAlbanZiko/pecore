#include "cc_internal.h"

void *CC_Malloc(uint64_t size)
{
	void *ptr = malloc(size);
	if (ptr == NULL)
	{
		CC_Log_Error("Failed to allocate memory of size %llu", size);
	}
	return ptr;
}

void *CC_Realloc(void *ptr, uint64_t size)
{
	void *newPtr = realloc(ptr, size);
	if (newPtr == NULL)
	{
		CC_Log_Error("Failed to reallocate memory of size %llu", size);
	}
	return newPtr;
}

void CC_Free(void *ptr)
{
	free(ptr);
}
 