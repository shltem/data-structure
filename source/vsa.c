#include <assert.h>/*assert*/
#include "utilities.h"/*word size*/
#include <stdio.h>
#include "vsa.h"

#define END_OF_BLOCKS (0xFFFFFFFF)
#define SIGNATURE (0xDEADBEEF)
#define ABS(X) (0 < X ? X : -X)


typedef struct block_header block_t;

 struct block_header
{
	long block_size; 
	#ifndef NDEBUG
	size_t signature;
	#endif
};

static void DefragmentPool(vsa_t *allocator);
static void InitBlockHeader(block_t *block, long block_size);

vsa_t *VsaInit(void *memory_pool, size_t pool_size)
{

	block_t *pool_head = NULL;
	block_t *end_pool = NULL;

	assert(NULL != memory_pool);

	pool_head = (block_t *)memory_pool;
	end_pool = (block_t *)((char *)memory_pool + (pool_size - sizeof(block_t)));
	
	InitBlockHeader(pool_head, pool_size - sizeof(vsa_t) * 2);
	InitBlockHeader(end_pool, END_OF_BLOCKS);
	return (vsa_t *)pool_head;
}

void *VsaAllocate(vsa_t *allocator, size_t block_size)
{

	vsa_t *current_header = allocator;
	vsa_t *next_header = NULL;
	long long_block_size = (long)block_size;
	assert(NULL != allocator);	
	
			
	if(0 != long_block_size % WORD_SIZE)
	{
		long_block_size += WORD_SIZE - long_block_size % WORD_SIZE;
	}
	
	if(long_block_size > current_header->block_size)
	{
		DefragmentPool(allocator);
	}
	
	while((END_OF_BLOCKS != current_header->block_size) && (long_block_size > current_header->block_size))
	{	
		current_header = (block_t *)((char *)current_header + ABS(current_header->block_size) + sizeof(block_t));
	}
	
	if(END_OF_BLOCKS == current_header->block_size)
	{
		return NULL;
	}
	
	else if ((long)(long_block_size + sizeof(block_t)) < (current_header->block_size))
	{	
		next_header = (block_t *)((char *)(current_header) + (long_block_size + sizeof(block_t)));
		InitBlockHeader(next_header, current_header->block_size - (long_block_size + sizeof(block_t)));
		InitBlockHeader(current_header, (long_block_size * -1));
	}
	else
	{
		InitBlockHeader(current_header, (current_header->block_size *-1));
	}
	
	return (void *)((char *)current_header + sizeof(block_t));
}

void VsaFree(void *to_free)
{
	vsa_t *header = NULL;
	
	assert(NULL != to_free);	
	
	header = (vsa_t *)((char *)to_free - sizeof(block_t));
	#ifndef NDEBUG
	assert(header->signature == SIGNATURE);
	#endif
	
	header->block_size = (header->block_size * -1);
}

size_t VsaLargestChunkAvailable(vsa_t *allocator)
{
	long max_chank = 0;
	block_t *current_header = NULL;
	
	assert(NULL != allocator);	
	
	current_header = (block_t *)allocator;
	max_chank = current_header->block_size;
	DefragmentPool(allocator);
	
	while (END_OF_BLOCKS != current_header->block_size)
	{
		if(current_header->block_size > max_chank )
		{
			max_chank = current_header->block_size;
		}
		current_header = (vsa_t *)((char *)(current_header) + ABS(current_header->block_size) + sizeof(block_t));
	}
	
	return max_chank;
}


static void DefragmentPool(vsa_t *allocator)
{
	long united_block_size = 0;
	block_t *next_header = NULL;
	block_t *current_header = (block_t *)allocator;
	
	while(END_OF_BLOCKS != current_header->block_size)
	{
		united_block_size = current_header->block_size;
		next_header = (block_t *)((char *)current_header + sizeof(block_t) + ABS(current_header->block_size));

		if((0 < current_header->block_size) && (0 < next_header->block_size) && (END_OF_BLOCKS != next_header->block_size))
		{
			united_block_size += next_header->block_size + sizeof(block_t);
			InitBlockHeader(current_header, united_block_size);
			continue;
		}
		current_header = next_header;
	}
}

static void InitBlockHeader(block_t *block, long block_size)
{
	block->block_size = block_size;
	#ifndef NDEBUG
	block->signature = SIGNATURE;
	#endif

}
