#ifndef _C_CORE_INTERNAL_H_
#define _C_CORE_INTERNAL_H_

#include <c_core.h>

// errors
void CC_Log_Log(uint64_t level, const char *file, uint64_t line, const char *format, ...);

typedef struct CC_ArrayList
{
    uint64_t elementSize;
    uint64_t countCurrent;
    uint64_t countMemory;
    uint8_t *data;
} CC_ArrayList;

typedef struct CC_PointerSet
{
    uint64_t countCurrent;
    uint64_t countMemory;
    void **pointers;
} CC_PointerSet;

typedef struct CC_UnorderedSet
{
	uint64_t elementSize;
	uint64_t countCurrent;
	uint64_t countMemory;
	uint8_t *data;
} CC_UnorderedSet;

typedef struct CC_HashMap
{
	uint64_t sizeOfElement;
	uint64_t countMemory;
	uint64_t countCurrent;
	CC_HashMap_Element *data;
} CC_HashMap;

typedef struct CC_HashMapString
{
	CC_PointerSet *string_collection;
	uint64_t sizeOfElement;
	uint64_t countMemory;
	uint64_t countCurrent;
	CC_HashMapString_Element *data;
} CC_HashMapString;

typedef struct _CC_HashMapString_Element
{
	char *name;
	uint8_t data[];
} _CC_HashMapString_Element;

typedef struct CC_Stack
{
	uint64_t sizeOfElement;
	uint64_t countCurrent;
	uint64_t countMemory;
	void *data;
} CC_Stack;


#endif