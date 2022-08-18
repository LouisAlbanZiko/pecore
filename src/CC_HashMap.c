#include "cc_internal.h"

CC_HashMap *CC_HashMap_Create(uint64_t sizeOfElement, uint64_t startCount)
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

	CC_HashMap *hash_map = CC_Malloc(sizeof(*hash_map));

	hash_map->sizeOfElement = sizeOfElement;
	hash_map->countCurrent = (uint64_t)0;
	hash_map->countMemory = (uint64_t)startCount;
	hash_map->data = CC_Malloc((sizeof(*(hash_map->data)) + sizeOfElement) * startCount);

	for (uint64_t i = 0; i < startCount; i++)
	{
		uint64_t *id = (uint64_t *)((uint8_t *)hash_map->data + i * (sizeof(CC_HashMap_Element) + hash_map->sizeOfElement));
		*id = hash_map->countMemory;
	}

	return hash_map;
}

void CC_HashMap_Destroy(CC_HashMap *map)
{
	CC_Free(map->data);
	CC_Free(map);
}

void CC_HashMap_SizeIncrease(CC_HashMap *map)
{
	uint64_t new_size = map->countMemory << 1;
	CC_HashMap *new_map = CC_HashMap_Create(map->sizeOfElement, new_size);

	for (CC_HashMap_Element *e = CC_HashMap_IteratorBegin(map); e != CC_HashMap_IteratorEnd(map); e = CC_HashMap_IteratorNext(map, e))
	{
		CC_HashMap_Insert(new_map, e->id, (void *)e->data);
	}

	CC_Free(map->data);
	*map = *new_map;
	CC_Free(new_map);
}

void CC_HashMap_Insert(CC_HashMap *map, uint64_t id, const void *data)
{
	if (map->countCurrent == map->countMemory)
	{
		CC_HashMap_SizeIncrease(map);
	}
	uint64_t hash = CC_Hash(&id, sizeof(id), 0) & (map->countMemory - 1);
	uint64_t *id_in_map = (uint64_t *)((uint8_t *)map->data + hash * (sizeof(CC_HashMap_Element) + map->sizeOfElement));
	while(*id_in_map != map->countMemory && *id_in_map != id)
	{
		hash = (hash + (uint64_t)1) & (map->countMemory - 1);
		id_in_map = (uint64_t *)((uint8_t *)map->data + hash * (sizeof(CC_HashMap_Element) + map->sizeOfElement));
	}
	if(*id_in_map != id)
	{
		*id_in_map = id;
		map->countCurrent++;
	}
	uint8_t *data_in_map = (uint8_t *)id_in_map + sizeof(CC_HashMap_Element);
	memcpy(data_in_map, data, map->sizeOfElement);
}

const void *CC_HashMap_Remove(CC_HashMap *map, uint64_t id)
{
	uint64_t hash = CC_Hash(&id, sizeof(id), 0) & (map->countMemory - 1);
	uint64_t *id_in_map = (uint64_t *)((uint8_t *)map->data + hash * (sizeof(CC_HashMap_Element) + map->sizeOfElement));
	while (*id_in_map != map->countMemory)
	{
		if (*id_in_map == id)
		{
			*id_in_map = map->countMemory;
			map->countCurrent--;
			return (uint8_t *)id_in_map + sizeof(CC_HashMap_Element);
		}
		hash = (hash + (uint64_t)1) & (map->countMemory - 1);
		id_in_map = (uint64_t *)((uint8_t *)map->data + hash * (sizeof(CC_HashMap_Element) + map->sizeOfElement));
	}
	return NULL;
}

const void *CC_HashMap_Lookup(CC_HashMap *map, uint64_t id)
{
	uint64_t hash = CC_Hash(&id, sizeof(id), 0) & (map->countMemory - 1);
	uint64_t *id_in_map = (uint64_t *)((uint8_t *)map->data + hash * (sizeof(CC_HashMap_Element) + map->sizeOfElement));
	if (*id_in_map != map->countMemory)
	{
		while (*id_in_map != id)
		{
			hash = (hash + (uint64_t)1) & (map->countMemory - 1);
			id_in_map = (uint64_t *)((uint8_t *)map->data + hash * (sizeof(CC_HashMap_Element) + map->sizeOfElement));
			if (*id_in_map == map->countMemory)
				return NULL;
		}
		uint8_t *data_in_map = (uint8_t *)id_in_map + sizeof(CC_HashMapString_Element);
		return (const void *)data_in_map;
	}
	else
	{
		return NULL;
	}
}

uint64_t CC_HashMap_Count(CC_HashMap *map)
{
	return map->countCurrent;
}

CC_HashMap_Element *CC_HashMap_IteratorBegin(CC_HashMap *map)
{
	CC_HashMap_Element *prev = map->data;
	while (prev->id == map->countMemory && prev != CC_HashMap_IteratorEnd(map))
	{
		prev = (CC_HashMap_Element *)(((uint8_t *)prev) + sizeof(CC_HashMap_Element) + map->sizeOfElement);
	}
	return prev;
}

CC_HashMap_Element *CC_HashMap_IteratorNext(CC_HashMap *map, CC_HashMap_Element *prev)
{
	do
	{
		prev = (CC_HashMap_Element *)(((uint8_t *)prev) + sizeof(CC_HashMap_Element) + map->sizeOfElement);
	} while (prev->id == map->countMemory && prev != CC_HashMap_IteratorEnd(map));
	return prev;
}

CC_HashMap_Element *CC_HashMap_IteratorEnd(CC_HashMap *map)
{
	return (CC_HashMap_Element *)(((uint8_t *)map->data) + map->countMemory * (sizeof(CC_HashMap_Element) + map->sizeOfElement));
}
