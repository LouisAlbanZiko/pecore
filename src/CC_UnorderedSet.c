#include "cc_internal.h"

CC_UnorderedSet *CC_UnorderedSet_Create(uint64_t elementSize, uint64_t startCount)
{
	elementSize = (elementSize != (uint64_t)0) * (uint64_t)16 + elementSize;
	CC_UnorderedSet *set = CC_Malloc(sizeof(*set));

	set->elementSize = elementSize;
	set->countCurrent = 0;
	set->countMemory = startCount;
	set->data = CC_Malloc(set->elementSize * set->countMemory);

	return set;
}

void CC_UnorderedSet_Destroy(CC_UnorderedSet *set)
{
	CC_Free(set->data);
	CC_Free(set);
}

uint64_t CC_UnorderedSet_Count(CC_UnorderedSet *set)
{
	return set->countCurrent;
}

uint64_t CC_UnorderedSet_Is_empty(CC_UnorderedSet *set)
{
	return set->countCurrent == 0;
}

void CC_UnorderedSet_Insert(CC_UnorderedSet *set, const void *data)
{
	if (set->countCurrent == set->countMemory)
	{
		set->countMemory <<= 1;
		set->data = CC_Realloc(set->data, set->elementSize * set->countMemory);
	}
	memcpy((uint8_t *)set->data + (set->elementSize * set->countCurrent), data, set->elementSize);
	set->countCurrent++;
}

void CC_UnorderedSet_Remove(CC_UnorderedSet *set, uint64_t index)
{
	index = index % set->countCurrent;
	void *data = (uint8_t *)set->data + index * set->elementSize;
	memcpy(data, (uint8_t *)set->data + (set->countCurrent - 1) * set->elementSize, set->elementSize);
	set->countCurrent--;
}

void CC_UnorderedSet_Set(CC_UnorderedSet *set, uint64_t index, const void *data)
{
	index = index % set->countCurrent;
	memcpy((uint8_t *)set->data + (set->elementSize * index), data, set->elementSize);
}

const void *CC_UnorderedSet_Get(CC_UnorderedSet *set, uint64_t index)
{
	index = index % set->countCurrent;
	return (set->data + (set->elementSize * index));
}

void CC_UnorderedSet_Reset(CC_UnorderedSet *set)
{
	set->countCurrent = 0;
}

void *CC_UnorderedSet_IteratorBegin(CC_UnorderedSet *set)
{
	return set->data;
}

void *CC_UnorderedSet_IteratorNext(CC_UnorderedSet *set, void *iterator)
{
	return (uint8_t *)iterator + set->elementSize;
}

void *CC_UnorderedSet_IteratorEnd(CC_UnorderedSet *set)
{
	return set->data + set->elementSize * set->countCurrent;
}
