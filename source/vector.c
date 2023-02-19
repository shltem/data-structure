#include <string.h>/*memcpy*/
#include <stdlib.h>/*malloc*/
#include <assert.h>/*assert*/

#include "vector.h"

/*approved by omri*/


static int VectorReserveInsude(vector_t *vector, size_t new_capacity);
struct vector
{
	char *base;
	size_t size_of_element;
	size_t capacity;
	size_t size;
};


vector_t *VectorCreate(size_t initial_capacity, size_t element_size)
{
	vector_t *vector_ptointer =(vector_t *)malloc(sizeof(struct vector));
	if (NULL == vector_ptointer)
	{
		return NULL;
	}
	
	vector_ptointer -> base = (char *)malloc(initial_capacity*element_size);
	if (NULL == vector_ptointer)
	{
		free(vector_ptointer);
		return NULL;
	}
	vector_ptointer -> size_of_element = element_size;
	vector_ptointer -> capacity = initial_capacity;
	vector_ptointer -> size = 0;
	
	return vector_ptointer;
	
}

void VectorDestroy(vector_t *vector)
{
	assert(NULL != vector);
	free(vector -> base);
	free(vector);
}

int VectorPushBack(vector_t *vector, const void *element_to_push)
{
	
	assert(NULL != element_to_push);
	assert(NULL != vector);

	if ((vector -> size) == (vector -> capacity))
	{
		int flag = 0;
		flag = VectorReserveInsude(vector, vector -> capacity*2);
		if (0 != flag)
		{
			return -1;
		}
	}
	
	memcpy(vector -> base + 
			(vector -> size)*(vector -> size_of_element), 
			element_to_push, vector -> size_of_element);

	++(vector -> size);
	
	return 0;
}

void *VectorPopBack(vector_t *vector)
{
	assert(NULL != vector);
	
	--(vector -> size);
	if (((vector -> size)*4) < vector -> capacity)
	{	
		int flag = 0;
		flag = VectorReserveInsude(vector, ((vector -> capacity)/2));
		if (0 != flag)
		{
			return NULL;
		}
	}	
	return (vector -> base + ((vector -> size))*(vector -> size_of_element));
		
}

void *VectorAccessAt(const vector_t *vector, size_t index)
{
	assert(NULL != vector);
	
	return (vector -> base +index*(vector -> size_of_element));
}

int VectorReserve(vector_t *vector, size_t new_capacity)
{
	int flag  = 0;
	assert(NULL != vector);
	flag = VectorReserveInsude(vector, new_capacity);
	if (0 != flag)
	{
		return -1;
	}
	return 0;
}

size_t VectorGetSize(const vector_t *vector)
{
	assert(NULL != vector);
	
	return (vector -> size);
}

size_t VectorGetCapacity(const vector_t *vector)
{
	assert(NULL != vector);
	
	return (vector -> capacity);
}

int VectorIsEmpty(const vector_t *vector)
{
	assert(NULL != vector);
	
	return (0 == vector -> size);
}

int VectorShrinkToFit(vector_t *vector)
{
	int flag = 0;
	assert(NULL != vector);
	
		flag = VectorReserveInsude(vector, (vector -> size));
		if (0 != flag)
		{
			return -1;
		}
		return 0;
}
static int VectorReserveInsude(vector_t *vector, size_t new_capacity)
{
	
	vector -> capacity = new_capacity; 
	vector -> base = (char *)realloc(vector -> base, 
									new_capacity*(vector -> size_of_element));
	if (NULL == vector -> base)
	{
		return -1;
	}
	return 0;
}

