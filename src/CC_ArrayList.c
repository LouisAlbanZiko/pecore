#include "cc_internal.h"

CC_ArrayList *CC_ArrayList_Create(uint64_t elementSize, uint64_t startCount)
{
	elementSize = elementSize / sizeof(uint64_t) * sizeof(uint64_t) + 1;
	startCount += (startCount == 0) * 16;

	CC_ArrayList* list = CC_Malloc(sizeof(*list));

	list->countCurrent = 0;
	list->countMemory = startCount;
	list->elementSize = elementSize;
	list->data = CC_Malloc(list->elementSize * list->countMemory);

	return list;
}

void CC_ArrayList_Destroy(CC_ArrayList *list)
{
	CC_Free(list->data);
	CC_Free(list);
}

void CC_ArrayList_Insert(CC_ArrayList *list, const void *data)
{
	if (list->countCurrent == list->countMemory)
	{
		list->countMemory <<= 1;
		list->data = CC_Realloc(list->data, list->countMemory * list->elementSize);
	}
	memcpy(list->data + list->countCurrent * list->elementSize, data, list->elementSize);
	list->countCurrent++;
}

void CC_ArrayList_InsertAt(CC_ArrayList *list, const void *data, uint64_t index)
{
	if (index <= list->countCurrent)
	{
		if (list->countCurrent == list->countMemory)
		{
			list->countMemory <<= 1;
			list->data = CC_Realloc(list->data, list->countMemory * list->elementSize);
		}
		for (uint64_t i = list->countCurrent; i > index; i--)
		{
			memcpy(list->data + i * list->elementSize, list->data + (i - 1) * list->elementSize, list->elementSize);
		}
		memcpy(list->data + index * list->elementSize, data, list->elementSize);
		list->countCurrent++;
	}
	else
	{
		// warn
	}
}

void CC_ArrayList_RemoveAt(CC_ArrayList *list, uint64_t index)
{
	if (index < list->countCurrent)
	{
		for (uint64_t i = index; i < list->countCurrent; i++)
		{
			memcpy(list->data + (i) * list->elementSize, list->data + (i + 1) * list->elementSize, list->elementSize);
		}
		list->countCurrent--;
	}
	else
	{
		// warn
	}
}

const void *CC_ArrayList_Get(const CC_ArrayList *list, uint64_t index)
{
	if (index >= list->countCurrent)
	{
		return NULL;
	}
	return list->data + index * list->elementSize;
}


void CC_ArrayList_Set(CC_ArrayList *list, const void *data, uint64_t index)
{
	if (index < list->countCurrent)
	{
		memcpy(list->data + index * list->elementSize, data, list->elementSize);
	}
	else
	{
		// warn
	}
}

void CC_ArrayList_Empty(CC_ArrayList *list)
{
	list->countCurrent = 0;
}

uint64_t CC_ArrayList_Count(const CC_ArrayList *list)
{
	return list->countCurrent;
}

uint64_t CC_ArrayList_IsEmpty(const CC_ArrayList *list)
{
	return list->countCurrent == 0;
}

uint64_t CC_ArrayList_IndexOf(const CC_ArrayList *list, const void *data)
{
	for (uint64_t i = 0; i < list->countCurrent; i++)
	{
		if (memcmp(list->data + i * list->elementSize, data, list->elementSize) == 0)
		{
			return i;
		}
	}
	return list->countCurrent;
}

const void *CC_ArrayList_IteratorBegin(const CC_ArrayList *arrayList)
{
    return (void *)arrayList->data;
}

const void *CC_ArrayList_IteratorNext(const CC_ArrayList *arrayList, const void *prev)
{
    return (uint8_t *)prev + arrayList->elementSize;
}

const void *CC_ArrayList_IteratorEnd(const CC_ArrayList *arrayList)
{
	return arrayList->data + arrayList->countCurrent * arrayList->elementSize;
}

char *CC_ArrayList_ToString(const CC_ArrayList *list, CC_Func_ToString func)
{
	CC_StringBuffer *buffer = CC_StringBuffer_Create(CC_ArrayList_Count(list) * 2);

	CC_StringBuffer_InsertString(buffer, "[");
	for (const void *element = CC_ArrayList_IteratorBegin(list); element < CC_ArrayList_IteratorEnd(list); element = CC_ArrayList_IteratorNext(list, element))
	{
		CC_StringBuffer_InsertChar(buffer, ' ');

		char *element_string = func(element);
		CC_StringBuffer_InsertString(buffer, element_string);
		CC_Free(element_string);
	}
	CC_StringBuffer_InsertString(buffer, " ]");

	return CC_StringBuffer_ToStringAndDestroy(buffer, NULL);
}
