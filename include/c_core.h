#ifndef C_CORE_H
#define C_CORE_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

// errors
typedef enum CC_Log_Level
{
	CC_LOG_LEVEL_TRACE = 0,
	CC_LOG_LEVEL_INFO,
	CC_LOG_LEVEL_WARN,
	CC_LOG_LEVEL_ERROR
} CC_Log_Level;

#define CC_Log_Trace(...)	{ CC_Log_Log(CC_LOG_LEVEL_TRACE, __FILE__, __LINE__, __VA_ARGS__); }
#define CC_Log_Info(...)	{ CC_Log_Log(CC_LOG_LEVEL_INFO,  __FILE__, __LINE__, __VA_ARGS__); }
#define CC_Log_Warn(...)	{ CC_Log_Log(CC_LOG_LEVEL_WARN,  __FILE__, __LINE__, __VA_ARGS__); }
#define CC_Log_Error(...)	{ CC_Log_Log(CC_LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__); if(CC_Log_IsFileSet()) CC_Log_CloseFile(); exit(1); }

void CC_Log_SetFile(const char *path);
uint64_t CC_Log_IsFileSet();
void CC_Log_CloseFile();
void CC_Log_SetLevel(CC_Log_Level level);

// memory
void *CC_Malloc(uint64_t size);
void *CC_Realloc(void *ptr, uint64_t size);
void CC_Free(void *ptr);

// hash function
uint32_t hashlittle(const void *key, size_t length, uint32_t initval);
uint64_t CC_Hash(const void *key, size_t length, uint64_t initval);

// CC_String
uint64_t CC_String_Length(const char *string);
uint64_t CC_String_Compare(const char *s1, const char *s2);
int64_t CC_String_Find(const char *string, char c);
uint64_t CC_String_ToUint64(const char *string);
char *CC_String_Copy(const char *string);
char *CC_String_CopyWithLength(const char *string, uint64_t length);
void CC_String_Destroy(char *string);
char *CC_String_Reverse(const char *string);

typedef char *(CC_Func_ToString)(const void *data);
char *CC_Uint64_ToString(const void *uint64_ptr);

// CC_StringBuffer
typedef struct CC_StringBuffer
{
	uint64_t countCurrent;
	uint64_t countMemory;
	char *data;
} CC_StringBuffer;

CC_StringBuffer *CC_StringBuffer_Create(uint64_t start_length);
void CC_StringBuffer_Destroy(CC_StringBuffer *buffer);

void CC_StringBuffer_InsertChar(CC_StringBuffer *buffer, char c);
void CC_StringBuffer_InsertString(CC_StringBuffer *buffer, const char *string);
void CC_StringBuffer_InsertSubString(CC_StringBuffer *buffer, const char *string, uint64_t length);
char *CC_StringBuffer_ToStringAndDestroy(CC_StringBuffer *buffer, uint64_t *length);

// CC_ArrayList
typedef struct CC_ArrayList CC_ArrayList;

CC_ArrayList *CC_ArrayList_Create(uint64_t elementSize, uint64_t startCount);
void CC_ArrayList_Destroy(CC_ArrayList *arrayList);

void CC_ArrayList_Insert(CC_ArrayList *arrayList, const void *data);
void CC_ArrayList_InsertAt(CC_ArrayList *arrayList, const void *data, uint64_t index);
void CC_ArrayList_RemoveAt(CC_ArrayList *arrayList, uint64_t index);
const void *CC_ArrayList_Get(const CC_ArrayList *arrayList, uint64_t index);
void CC_ArrayList_Set(CC_ArrayList *arrayList, const void *data, uint64_t index);
void CC_ArrayList_Empty(CC_ArrayList *arrayList);
uint64_t CC_ArrayList_Count(const CC_ArrayList *arrayList);
uint64_t CC_ArrayList_IsEmpty(const CC_ArrayList *arrayList);
uint64_t CC_ArrayList_IndexOf(const CC_ArrayList *arrayList, const void *data);

const void *CC_ArrayList_IteratorBegin(const CC_ArrayList *arrayList);
const void *CC_ArrayList_IteratorNext(const CC_ArrayList *arrayList, const void *prev);
const void *CC_ArrayList_IteratorEnd(const CC_ArrayList *arrayList);

char *CC_ArrayList_ToString(const CC_ArrayList *arrayList, CC_Func_ToString func);

// CC_PointerSet
typedef struct CC_PointerSet CC_PointerSet;

CC_PointerSet *CC_PointerSet_Create();
void CC_PointerSet_Destroy(CC_PointerSet *set);

void CC_PointerSet_Register(CC_PointerSet *set, void *ptr);
void CC_PointerSet_Empty(CC_PointerSet *set);

void *CC_PointerSet_Malloc(CC_PointerSet *set, uint64_t size);
void *CC_PointerSet_Realloc(CC_PointerSet *set, void *ptr, uint64_t size);
void CC_PointerSet_Free(CC_PointerSet *set, void *ptr);

// CC_UnorderedSet
typedef struct CC_UnorderedSet CC_UnorderedSet;

CC_UnorderedSet *CC_UnorderedSet_Create(uint64_t elementSize, uint64_t startCount);
void CC_UnorderedSet_Destroy(CC_UnorderedSet *set);

uint64_t CC_UnorderedSet_Count(CC_UnorderedSet *set);
uint64_t CC_UnorderedSet_Is_empty(CC_UnorderedSet *set);

void CC_UnorderedSet_Insert(CC_UnorderedSet *set, const void *data);
void CC_UnorderedSet_Remove(CC_UnorderedSet *set, uint64_t index);
void CC_UnorderedSet_Reset(CC_UnorderedSet *set);

void CC_UnorderedSet_Set(CC_UnorderedSet *set, uint64_t index, const void *data);
const void *CC_UnorderedSet_Get(CC_UnorderedSet *set, uint64_t index);

void *CC_UnorderedSet_IteratorBegin(CC_UnorderedSet *set);
void *CC_UnorderedSet_IteratorNext(CC_UnorderedSet *set, void *iterator);
void *CC_UnorderedSet_IteratorEnd(CC_UnorderedSet *set);

// CC_HashMap
typedef struct CC_HashMap CC_HashMap;

typedef struct CC_HashMap_Element
{
	const uint64_t id;
	const uint8_t data[];
} CC_HashMap_Element;

CC_HashMap *CC_HashMap_Create(uint64_t sizeOfElement, uint64_t startCount);
void CC_HashMap_Destroy(CC_HashMap *map);

void CC_HashMap_Insert(CC_HashMap *map, uint64_t id, const void *data);
const void *CC_HashMap_Remove(CC_HashMap *map, uint64_t id);
const void *CC_HashMap_Lookup(CC_HashMap *map, uint64_t id);

uint64_t CC_HashMap_Count(CC_HashMap *map);

CC_HashMap_Element *CC_HashMap_IteratorBegin(CC_HashMap *map);
CC_HashMap_Element *CC_HashMap_IteratorNext(CC_HashMap *map, CC_HashMap_Element *prev);
CC_HashMap_Element *CC_HashMap_IteratorEnd(CC_HashMap *map);

// CC_HashMapString
typedef struct CC_HashMapString CC_HashMapString;

typedef struct CC_HashMapString_Element
{
	const char *const name;
	const uint8_t data[];
} CC_HashMapString_Element;

CC_HashMapString *CC_HashMapString_Create(uint64_t sizeOfElement, uint64_t startCount);
void CC_HashMapString_Destroy(CC_HashMapString *map);

void CC_HashMapString_Insert(CC_HashMapString *map, const char *name, const void *data);
const void *CC_HashMapString_Remove(CC_HashMapString *map, const char *name);
const void *CC_HashMapString_Lookup(CC_HashMapString *map, const char *name);

uint64_t CC_HashMapString_Count(CC_HashMapString *map);

CC_HashMapString_Element *CC_HashMapString_IteratorBegin(CC_HashMapString *map);
CC_HashMapString_Element *CC_HashMapString_IteratorNext(CC_HashMapString *map, CC_HashMapString_Element *prev);
CC_HashMapString_Element *CC_HashMapString_IteratorEnd(CC_HashMapString *map);

// cc_stack
typedef struct CC_Stack CC_Stack;

CC_Stack *cc_stack_create(uint64_t sizeOfElement, uint64_t startCount);
void cc_stack_destroy(CC_Stack *stack);

uint64_t cc_stack_count(CC_Stack *stack);
uint64_t cc_stack_is_empty(CC_Stack *stack);
void cc_stack_reset(CC_Stack *stack);

void cc_stack_push(CC_Stack *stack, void *data);
const void *cc_stack_peek(CC_Stack *stack);
const void *cc_stack_pop(CC_Stack *stack);

const void *cc_stack_iterator_begin(CC_Stack *stack);
const void *cc_stack_iterator_next(CC_Stack *stack, const void *prev);
const void *cc_stack_iterator_end(CC_Stack *stack);

#endif