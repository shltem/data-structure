#include <stdio.h>/*printf*/

#include "vsa.h"


int main()
{
	char pool[200];
	size_t befor_alooc = 0;
	size_t after_alooc = 0;
	size_t after_free = 0;
	vsa_t *vsa = NULL;
	void *ptr1 = NULL;
	void *ptr2 = NULL;
	void *ptr3 = NULL;
	void *ptr4 = NULL;
	void *ptr5 = NULL;
	
	vsa = VsaInit(pool, 200);
	befor_alooc = VsaLargestChunkAvailable(vsa);
	
	ptr1 = VsaAllocate(vsa, 8);
	ptr2 = VsaAllocate(vsa, 16);
	ptr3 = VsaAllocate(vsa, 8);
	ptr4 = VsaAllocate(vsa, 8);
	ptr5 = VsaAllocate(vsa, 16);
	
	after_alooc = VsaLargestChunkAvailable(vsa);
	
	if (befor_alooc == 168 && after_alooc == 32)
	{
		printf("VsaIint Allocate & LargestChunkAvailable are working\n");
	}
	else
	{
		printf("VsaAllocate & LargestChunkAvailable are NOT working \n");
	}
	VsaFree(ptr2);
	VsaFree(ptr3);
	after_free = VsaLargestChunkAvailable(vsa);
	
	if (40 == after_free)
	{
		printf("Vsafree works\n");
	}
	else
	{
		printf("Vsafree works NOT working \n");
	}
	return 0;
}
