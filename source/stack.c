#include <string.h>/*memcpy*/
#include <stdlib.h>/*malloc*/
#include <assert.h>/*assert*/
#include "../include/stack.h"

/*approved by omri*/

struct stack
{
	char * top;
	size_t size_of_element;
	size_t capacity;
	size_t size;
};

stack_ptr_t StackCreate(size_t capacity, size_t element_size)
{
	stack_ptr_t stack_pointer =
	(stack_ptr_t)malloc(sizeof(struct stack)+capacity*element_size);
	if (NULL == stack_pointer)
	{
		return NULL;
	}
	
	stack_pointer -> top = ((char *)stack_pointer+(sizeof(struct stack)));
	stack_pointer -> size_of_element = element_size;
	stack_pointer -> capacity = capacity;
	stack_pointer -> size = 0;
	
	return stack_pointer;
	
}
void StackPush(stack_ptr_t stack, const void *element_to_push)
{
		assert(NULL != element_to_push);
		assert(NULL != stack);
		
		memcpy(stack -> top, element_to_push, stack -> size_of_element);
		(stack -> top) += (stack -> size_of_element);
		++(stack -> size);
}

void *StackPop(stack_ptr_t stack)
{
	assert(NULL != stack);
	
	(stack -> top) -= (stack -> size_of_element);
	--(stack -> size);
	
	return stack -> top;
}


void StackDestroy(stack_ptr_t stack)
{
	assert(NULL != stack);
	free(stack);	
}


int StackIsEmpty(const stack_ptr_t stack)
{
	assert(NULL != stack);
	
	return ( 0 == stack -> size);
}

size_t StackGetSize(const stack_ptr_t stack)
{
	assert(NULL != stack);
	return stack -> size ;
}

size_t StackGetCapacity(const stack_ptr_t stack)
{
	assert(NULL != stack);
	return stack -> capacity ;
}

void *StackPeek(const stack_ptr_t stack)
{
	assert(NULL != stack);
	
	return ((stack -> top)-(stack -> size_of_element));
}

