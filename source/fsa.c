#include <assert.h>/*assert*/
#include <stdio.h>

#include "utilities.h"/*word size*/

#define LAST_BLOCK (1)
#define END_OF_FREE_BLOCKS (0)

#include "fsa.h"

struct fsa
{ 		
	size_t block_size;
	size_t offset_next_available;
};


size_t FSASuggestSize(size_t blocksize, size_t num_of_blocks)
{
	size_t size_to_return = 0;

	blocksize  = blocksize + ((-blocksize) & (WORD_SIZE-1));
	size_to_return = sizeof(fsa_t) + blocksize * num_of_blocks;

	return size_to_return;
}
                     
fsa_t *FSAInit(void *memory_pool, size_t pool_size, size_t block_size)
{
	fsa_t *fsa = NULL;
	size_t num_of_blocks = 0;
	size_t runner = 0;
	size_t index = 0 ;
	assert(NULL != memory_pool);
	block_size = block_size + ((-block_size) & (WORD_SIZE-1));

	if(0 != block_size % WORD_SIZE)
	{
		block_size += WORD_SIZE - block_size % WORD_SIZE;
	}

	assert(sizeof(fsa_t) + block_size< pool_size);
	
	fsa = (fsa_t *)memory_pool; 
	fsa->block_size = block_size;
	num_of_blocks = (pool_size - sizeof(fsa_t)) / block_size;
	fsa->offset_next_available = sizeof(fsa_t);
	runner = fsa->offset_next_available;
	for (index = 0; index < (num_of_blocks - LAST_BLOCK); ++index)
	{
		*(size_t *)((size_t)fsa + runner) = runner + block_size;
		runner += block_size;
	}	
	*(size_t *)((size_t)fsa + runner) = END_OF_FREE_BLOCKS;

	return fsa;
}

void *FSAAllocate(fsa_t *allocator)
{

	void *block_to_allocate = NULL;

	assert(NULL != allocator);

	if (0 == FSACountFree(allocator))
	{
		return NULL;
	}

	block_to_allocate = (void *)((size_t)allocator + allocator->offset_next_available);
	allocator->offset_next_available = *(size_t *)block_to_allocate;

	return block_to_allocate;
}

void FSAFree(fsa_t *allocator, void *to_free)
{
	assert(NULL != allocator);
	assert(NULL != to_free);

	*(size_t *)to_free = allocator->offset_next_available;
	allocator->offset_next_available = ((size_t)to_free - (size_t)allocator);		
}

size_t FSACountFree(const fsa_t *allocator)
{
	size_t counter = 0;
	size_t runner_offset = 0;
	
	assert(NULL != allocator);
	
	runner_offset = allocator->offset_next_available;
	
	while(END_OF_FREE_BLOCKS != runner_offset)
	{
		runner_offset = *(size_t *)((size_t)allocator + runner_offset);
		++counter;
	}
	
	return counter;
}

