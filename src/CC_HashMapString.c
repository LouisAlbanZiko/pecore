#include "cc_internal.h"

CC_HashMapString *CC_HashMapString_Create(uint64_t sizeOfElement, uint64_t startCount)
{
	startCount = (startCount == 0) * 16 + startCount;
	// round up to the next highest power of 2
	startCount--;
	startCount |= startCount >> 1;
	startCount |= startCount >> 2;
	startCount |= startCount >> 4;
	startCount |= startCount >> 8;
	startCount |= startCount >> 16;
	startCount++;
	//////////////////////////////////////////
	sizeOfElement = (sizeOfElement / sizeof(void *) + ((sizeOfElement % sizeof(void *)) != 0)) * sizeof(void *);

	CC_HashMapString *hash_map = CC_Malloc(sizeof(*hash_map));

	hash_map->string_collection = CC_PointerSet_Create();
	hash_map->sizeOfElement = sizeOfElement;
	hash_map->countCurrent = (uint64_t)0;
	hash_map->countMemory = (uint64_t)startCount;
	hash_map->data = CC_Malloc((sizeof(*(hash_map->data)) + sizeOfElement) * startCount);

	for (uint64_t i = 0; i < startCount; i++)
	{
		const char **name = (const char **)((uint8_t *)hash_map->data + i * (sizeof(CC_HashMapString_Element) + hash_map->sizeOfElement));
		*name = NULL;
	}

	return hash_map;
}

void CC_HashMapString_Destroy(CC_HashMapString *map)
{
	CC_PointerSet_Destroy(map->string_collection);
	CC_Free(map->data);
	CC_Free(map);
}

void CC_HashMapString_SizeIncrease(CC_HashMapString *map)
{
	uint64_t new_size = map->countMemory << 1;
	CC_HashMapString *new_map = CC_HashMapString_Create(map->sizeOfElement, new_size);

	for (CC_HashMapString_Element *e = CC_HashMapString_IteratorBegin(map); e != CC_HashMapString_IteratorEnd(map); e = CC_HashMapString_IteratorNext(map, e))
	{
		CC_HashMapString_Insert(new_map, e->name, (void *)e->data);
	}

	CC_PointerSet_Destroy(map->string_collection);
	CC_Free(map->data);
	*map = *new_map;
	CC_Free(new_map);
}

void CC_HashMapString_Insert(CC_HashMapString *map, const char *name, const void *data)
{
	if (map->countCurrent == map->countMemory)
	{
		CC_HashMapString_SizeIncrease(map);
	}
	uint64_t hash = CC_Hash(name, CC_String_Length(name), 0) & (map->countMemory - 1);
	char **name_in_map = (char **)((uint8_t *)map->data + hash * (sizeof(CC_HashMapString_Element) + map->sizeOfElement));
	while (*name_in_map != NULL)
	{
		if (strcmp(*name_in_map, name) == 0)
		{
			uint8_t *data_in_map = (uint8_t *)name_in_map + sizeof(CC_HashMapString_Element);
			memcpy(data_in_map, data, map->sizeOfElement);
			return;
		}
		hash = (hash + 1) & (map->countMemory - 1);
		name_in_map = (char **)((uint8_t *)map->data + hash * (sizeof(CC_HashMapString_Element) + map->sizeOfElement));
	}
	char *name_copy = CC_String_Copy(name);
	CC_PointerSet_Register(map->string_collection, name_copy);
	*name_in_map = name_copy;
	uint8_t *data_in_map = (uint8_t *)((uint8_t *)name_in_map + sizeof(CC_HashMapString_Element));
	memcpy(data_in_map, data, map->sizeOfElement);
	map->countCurrent++;
}

const void *CC_HashMapString_Remove(CC_HashMapString *map, const char *name)
{
	uint64_t hash = CC_Hash(name, CC_String_Length(name), 0) & (map->countMemory - 1);
	char **name_in_map = (char **)((uint8_t *)map->data + hash * (sizeof(CC_HashMapString_Element) + map->sizeOfElement));
	while (*name_in_map != NULL)
	{
		if (CC_String_Compare(*name_in_map, name))
		{
			CC_String_Destroy(*name_in_map);
			*name_in_map = NULL;
			map->countCurrent--;
			return (uint8_t *)name_in_map + sizeof(CC_HashMapString_Element);
		}
		hash = (hash + (uint64_t)1) & (map->countMemory - 1);
		name_in_map = (char **)((uint8_t *)map->data + hash * (sizeof(CC_HashMapString_Element) + map->sizeOfElement));
	}
	return NULL;
}

const void *CC_HashMapString_Lookup(CC_HashMapString *map, const char *name)
{
	uint64_t hash = CC_Hash(name, CC_String_Length(name), 0) & (map->countMemory - 1);
	char **name_in_map = (char **)((uint8_t *)map->data + hash * (sizeof(CC_HashMapString_Element) + map->sizeOfElement));
	if (*name_in_map != NULL)
	{
		while (!CC_String_Compare(*name_in_map, name))
		{
			hash = (hash + 1) & (map->countMemory - 1);
			name_in_map = (char **)((uint8_t *)map->data + hash * (sizeof(CC_HashMapString_Element) + map->sizeOfElement));
			if (*name_in_map == NULL)
				return NULL;
		}
		uint8_t *data_in_map = (uint8_t *)name_in_map + sizeof(CC_HashMapString_Element);
		return (const void *)data_in_map;
	}
	else
	{
		return NULL;
	}
}

uint64_t CC_HashMapString_Count(CC_HashMapString *map)
{
	return map->countCurrent;
}

CC_HashMapString_Element *CC_HashMapString_IteratorBegin(CC_HashMapString *map)
{
	CC_HashMapString_Element *prev = map->data;
	while (prev->name == NULL && prev != CC_HashMapString_IteratorEnd(map))
	{
		prev = (CC_HashMapString_Element *)(((uint8_t *)prev) + sizeof(CC_HashMapString_Element) + map->sizeOfElement);
	}
	return prev;
}

CC_HashMapString_Element *CC_HashMapString_IteratorNext(CC_HashMapString *map, CC_HashMapString_Element *prev)
{
	do
	{
		prev = (CC_HashMapString_Element *)(((uint8_t *)prev) + sizeof(CC_HashMapString_Element) + map->sizeOfElement);
	} while (prev->name == NULL && prev != CC_HashMapString_IteratorEnd(map));
	return prev;
}

CC_HashMapString_Element *CC_HashMapString_IteratorEnd(CC_HashMapString *map)
{
	return (CC_HashMapString_Element *)(((uint8_t *)map->data) + map->countMemory * (sizeof(CC_HashMapString_Element) + map->sizeOfElement));
}

uint64_t CC_Hash(const void *key, size_t length, uint64_t initval)
{
	uint64_t result;
	uint32_t *ptr1_out = (uint32_t *)&result;
	uint32_t *ptr2_out = (ptr1_out + 1);

	*ptr1_out = hashlittle(key, length, (uint32_t)initval);
	*ptr2_out = hashlittle(key, length, *ptr1_out);

	return result;
}
