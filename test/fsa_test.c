#include <stdio.h>/*printf*/
#include <stddef.h>/*size_t*/
#include "utiletis.h"/*word size*/
#define FSA_SIZE (16)
#include "../include/fsa.h"

int main()
{	
	fsa_t *fsa = NULL ;
	char array[2000] = {0};
	size_t size_num = 176;
	size_t size_of_actual_block_size = 0;
	size_t size_of_block_demand = 13;
	size_t number_of_block = 10;
	size_t suggest_size_shoild_be = 0;
	size_t number_befor_alloc = 0;
	size_t number_after_alloc = 0;
	size_t number_after_free = 0;
	void *ptr1 = NULL;
	void *ptr2 = NULL;
	void *ptr3 = NULL;
	void *ptr4 = NULL;
	void *ptr5 = NULL;
	void *ptr6 = NULL;
	void *ptr7 = NULL;
	void *ptr8 = NULL;
	void *ptr9 = NULL;
	void *ptr10 = NULL;
	int after_alooc = 0;
	int after_free = 0;
	/* test FSASuggestSize*/
	
	if( 0 != size_of_block_demand % WORD_SIZE)
	{
		size_of_actual_block_size += size_of_block_demand + (WORD_SIZE - size_of_block_demand % WORD_SIZE);
	}
	
	suggest_size_shoild_be = FSA_SIZE+size_of_actual_block_size*number_of_block;
	 
	if(suggest_size_shoild_be == FSASuggestSize(13, 10))
	{
		printf("FSASuggestSize is working!!                V\n");
	}
	else
	{
		printf("FSASuggestSize is NOT working!!           x\n");
	}
	
	/*test FSACountFree*/
	fsa = FSAInit(array, size_num, size_of_block_demand);
	number_befor_alloc = number_of_block;
	ptr1 = FSAAllocate(fsa);
	ptr2 = FSAAllocate(fsa);
	ptr3 = FSAAllocate(fsa);
	ptr4 = FSAAllocate(fsa);
	ptr5 = FSAAllocate(fsa);
	ptr6 = FSAAllocate(fsa);
	ptr7 = FSAAllocate(fsa);
	ptr8 = FSAAllocate(fsa);
	ptr9 = FSAAllocate(fsa);
	ptr10 = FSAAllocate(fsa);
	
	number_after_alloc = FSACountFree(fsa);
	FSAFree(fsa, ptr1);
	FSAFree(fsa, ptr2);
	FSAFree(fsa, ptr3);
	FSAFree(fsa, ptr4);
	FSAFree(fsa, ptr5);
	number_after_free = FSACountFree(fsa);
	FSAFree(fsa, ptr6);
	FSAFree(fsa, ptr7);
	FSAFree(fsa, ptr8);
	FSAFree(fsa, ptr9);
	FSAFree(fsa, ptr10);

	if ( number_befor_alloc == 10 && number_after_alloc == 0 
							&& number_after_free == 5 )
	{
		printf("FSACountFree is working!!                  V\n");
	}
	else
	{
		printf("FSACountFree is NOT working!!             x\n");
	}

	/*test FSAAlloc and FSAFree*/

	ptr1 = FSAAllocate(fsa);
	*(int *)ptr1 = 5;
	after_alooc = *(int *)ptr1;
	FSAFree(fsa, ptr1);
	after_free = *(int *)ptr1;
	
	if ( after_alooc == 5 && after_free != 5)
							
	{
		printf("FSAAlloc and FSAFree are working!!         V\n");
	}
	else
	{
		printf("FSAAlloc and FSAFree are NOT working!!     x\n");
	}
	
		
	return 0;
}
