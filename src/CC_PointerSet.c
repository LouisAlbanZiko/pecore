#include "cc_internal.h"

CC_PointerSet *CC_PointerSet_Create()
{
	CC_PointerSet *set = CC_Malloc(sizeof(*set));

	set->countCurrent = 0;
	set->countMemory = 16;
	set->pointers = CC_Malloc(sizeof(*set->pointers) * set->countMemory);

	return set;
}

void CC_PointerSet_Destroy(CC_PointerSet *set)
{
	CC_PointerSet_Empty(set);
	CC_Free(set->pointers);
	CC_Free(set);
}

void CC_PointerSet_Register(CC_PointerSet *set, void *ptr)
{
	if (set->countCurrent == set->countMemory)
	{
		set->countMemory <<= 1;
		set->pointers = CC_Realloc(set->pointers, sizeof(*set->pointers) * set->countMemory);
	}
	set->pointers[set->countCurrent] = ptr;
	set->countCurrent++;
}

void CC_PointerSet_Empty(CC_PointerSet *set)
{
	for (uint64_t i = 0; i < set->countCurrent; i++)
	{
		CC_Free(set->pointers[i]);
	}
	set->countCurrent = 0;
}

uint64_t CC_PointerSet_IndexOf(CC_PointerSet *set, const void *ptr)
{
	for (uint64_t i = 0; i < set->countCurrent; i++)
	{
		if (ptr == set->pointers[i])
		{
			return i;
		}
	}
	return set->countCurrent;
}

void *CC_PointerSet_Malloc(CC_PointerSet *set, uint64_t size)
{
	void *ptr = CC_Malloc(size);
	CC_PointerSet_Register(set, ptr);
	return ptr;
}

void *CC_PointerSet_Realloc(CC_PointerSet *set, void *ptr, uint64_t size)
{
	uint64_t index = CC_PointerSet_IndexOf(set, ptr);
	if (index != set->countCurrent)
	{
		void *new_ptr = CC_Realloc(ptr, size);
		set->pointers[index] = new_ptr;
		return new_ptr;
	}
	else
	{
		return NULL;
	}
}

void CC_PointerSet_Free(CC_PointerSet *set, void *ptr)
{
	uint64_t index = CC_PointerSet_IndexOf(set, ptr);
	if (index != set->countCurrent)
	{
		CC_Free(ptr);
		set->pointers[index] = set->pointers[set->countCurrent];
		set->countCurrent--;
	}
}
