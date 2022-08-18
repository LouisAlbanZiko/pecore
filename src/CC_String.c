#include "cc_internal.h"

uint64_t CC_String_Length(const char *string)
{
	uint64_t length = 0;
	while(string[length] != '\0')
	{
		length++;
	}
	return length;
}

uint64_t CC_String_Compare(const char *s1, const char *s2)
{
	uint64_t index = 0;
	while(s1[index] != s2[index] && s1[index] != '\0' && s2[index] != '\0')
	{
		index++;
	}
	return s1[index] == s2[index];
}

int64_t CC_String_Find(const char *string, char c)
{
	int64_t index = 0;
	while(string[index] != '\0' && string[index] != c)
	{
		index++;
	}
	if(string[index] == c)
	{
		return index;
	}
	else
	{
		return -1;
	}
}

uint64_t CC_String_ToUint64(const char *string)
{
	uint64_t value = 0;
	while(*string >= '0' && *string <= '9')
	{
		value *= 10;
		value += *string - '0';
		string++;
	}
	return value;
}

char *CC_String_Copy(const char *string)
{
	uint64_t length = CC_String_Length(string);
	char *copy = CC_Malloc(length + 1);
	memcpy(copy, string, length);
	copy[length] = '\0';
	return copy;
}

char *CC_String_CopyWithLength(const char *string, uint64_t length)
{
	char *copy = CC_Malloc(length + 1);
	memcpy(copy, string, length);
	copy[length] = '\0';
	return copy;
}

void CC_String_Destroy(char *string)
{
	CC_Free(string);
}

char *CC_String_Reverse(const char *string)
{
	uint64_t len = CC_String_Length(string);
	char *copy = CC_Malloc(len + 1);
	for (uint64_t i = 0; i < len; i++)
	{
		copy[i] = string[len - i - 1];
	}
	copy[len] = '\0';
	return copy;
}

char *CC_Uint64_ToString(const void *nrPtr)
{
	uint64_t nr = *((uint64_t *)nrPtr);

	CC_StringBuffer *buffer = CC_StringBuffer_Create(32);
	
	do {
		char c = '0' + (nr % 10);
		CC_StringBuffer_InsertChar(buffer, c);
	} while (nr /= 10);

	char *stringReversed = CC_StringBuffer_ToStringAndDestroy(buffer, NULL);
	char *string = CC_String_Reverse(stringReversed);
	CC_Free(stringReversed);

	return string;
}
