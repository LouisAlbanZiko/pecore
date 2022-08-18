#include "cc_internal.h"

CC_Stack *cc_stack_create(uint64_t sizeOfElement, uint64_t startCount)
{
	startCount = (!startCount) * 32 + startCount;

	CC_Stack *stack = CC_Malloc(sizeof(*stack));

	stack->sizeOfElement = sizeOfElement;
	stack->countCurrent = 0;
	stack->countMemory = startCount;
	stack->data = CC_Malloc(stack->countMemory * stack->sizeOfElement);

	return stack;
}

void cc_stack_destroy(CC_Stack *stack)
{
	CC_Free(stack->data);
	CC_Free(stack);
}

uint64_t cc_stack_count(CC_Stack *stack)
{
	return stack->countCurrent;
}

uint64_t cc_stack_is_empty(CC_Stack *stack)
{
	return stack->countCurrent == 0;
}

void cc_stack_reset(CC_Stack *stack)
{
	stack->countCurrent = 0;
}

void cc_stack_push(CC_Stack *stack, void *data)
{
	if (stack->countCurrent == stack->countMemory)
	{
		stack->countMemory <<= 1;
		stack->data = CC_Realloc(stack->data, (uint64_t)stack->sizeOfElement * stack->countMemory);
	}
	memcpy((uint8_t *)stack->data + (stack->countCurrent * (uint64_t)stack->sizeOfElement), data, stack->sizeOfElement);
	stack->countCurrent++;
}

const void *cc_stack_peek(CC_Stack *stack)
{
	if (stack->countCurrent == 0)
	{
		return NULL;
	}
	else
	{
		uint8_t *data = stack->data;
		return data + (uint64_t)stack->sizeOfElement * (stack->countCurrent - 1);
	}
}

const void *cc_stack_pop(CC_Stack *stack)
{
	if (stack->countCurrent == 0)
	{
		return NULL;
	}
	else
	{
		uint8_t *data = stack->data;
		stack->countCurrent--;
		return data + (uint64_t)stack->sizeOfElement * stack->countCurrent;
	}
}

const void *cc_stack_iterator_begin(CC_Stack *stack)
{
	return ((uint8_t *)stack->data) + (stack->countCurrent - 1) * (uint64_t)stack->sizeOfElement;
}

const void *cc_stack_iterator_next(CC_Stack *stack, const void *prev)
{
	return ((const uint8_t *)prev) - (stack->sizeOfElement);
}
const void *cc_stack_iterator_end(CC_Stack *stack)
{
	return ((uint8_t *)stack->data) - (stack->sizeOfElement);
}