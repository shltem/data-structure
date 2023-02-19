#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */
#include "vsa.h"



static void TestInit();
static void TestAlloc();
static void TestFree();
static void TestLargestChunk();

int main()
{
    TestInit();
    TestAlloc();
    TestFree();
    TestLargestChunk();
    return (0);
}


static void TestInit()
{
    int pool_size = 100;
    int size_left = 0;
    vsa_t *vsa = NULL;
    
    void *pool = malloc(pool_size);
    vsa = VsaInit(pool, pool_size);
    size_left = VsaLargestChunkAvailable(vsa);
   
    if(68 == size_left)
    {
        printf("VsaInit working!                             SUCCESS!!!\n");
    }
    else
    {
        printf("VsaInit NOT working!                          FAIL!!!\n");
    }
    
    free(pool);
}

static void TestAlloc()
{
    size_t pool_size = 300;
    size_t addresses[10] = {0};
    int adress_index = 0;
    int is_working = 0;
    vsa_t *vsa = NULL;
    
    void *pool = malloc(pool_size);
    vsa = VsaInit(pool, pool_size);
    
    for(adress_index = 0; adress_index < 10; ++adress_index)
    {
        addresses[adress_index] = (size_t)VsaAllocate(vsa, 30);
    }
    
   for(adress_index = 0; adress_index < 9; ++adress_index)
    {
        if(adress_index < 5 && 0 == addresses[adress_index])
        {
            is_working += 1;
            break;
        }
        if(adress_index < 4 && (addresses[adress_index+1] - addresses[adress_index]) != 48) 
        {
            is_working += 2;
            break;
        }
        
        if(adress_index >= 5 && (addresses[adress_index] != 0))
        {
            is_working += 4;
            break;
        }
    }
    
    if(28 != VsaLargestChunkAvailable(vsa))
    {
        is_working += 8;
    }
    
    if(0 == is_working)
    {
       printf("VsaAllocate working                            SUCCESS!!!\n");
    }
    else
    {
        printf("VsaAllocate NOT working ERROR_CODE  = %d       FAIL!!!\n", is_working);
    }
    
    free(pool);
}

static void TestFree()
{
	size_t pool_size = 300;
    size_t addresses[10] = {0};
    int adress_index = 0;
    int is_working = 0;
    vsa_t *vsa = NULL;
    void *free1 = NULL;
    void *free2 = NULL;
    
    void *pool = malloc(pool_size);
    vsa = VsaInit(pool, pool_size);
    

 for(adress_index = 0; adress_index < 10; ++adress_index)
    {
        addresses[adress_index] = (size_t)VsaAllocate(vsa, 30);
    }
    
   for(adress_index = 0; adress_index < 9; ++adress_index)
    {
        if(adress_index < 5 && 0 == addresses[adress_index])
        {
            is_working += 1;
            break;
        }
        if(adress_index < 4 && (addresses[adress_index+1] - addresses[adress_index]) != 48) 
        {
            is_working += 2;
            break;
        }
        
        if(adress_index >= 5 && (addresses[adress_index] != 0))
        {
            is_working += 4;
            break;
        }
    }
    free1 = (void *)addresses[3];
    free2 = (void *)addresses[2];
    VsaFree(free1);
    VsaFree(free2);
    
    if(80 != VsaLargestChunkAvailable(vsa))
    {
        is_working += 8;
    }
    
    if(0 == is_working)
    {
         printf("VsaFree working                               SUCCESS!!!\n");
    }
    else
    {
         printf("VsaFree NOT working ERROR_CODE  = %d       FAIL!!!\n", is_working);
    }
    
    free(pool);
    
}

static void TestLargestChunk()
{
	size_t pool_size = 300;
    size_t addresses[10] = {0};
    int adress_index = 0;
    int is_working = 0;
    vsa_t *vsa = NULL;
    void *free1 = NULL;
    void *free2 = NULL;
    
    void *pool = malloc(pool_size);
    vsa = VsaInit(pool, pool_size);
    
  for(adress_index = 0; adress_index < 10; ++adress_index)
    {
        addresses[adress_index] = (size_t)VsaAllocate(vsa, 30);
    }
    
   for(adress_index = 0; adress_index < 9; ++adress_index)
    {
        if(adress_index < 5 && 0 == addresses[adress_index])
        {
            is_working += 1;
            break;
        }
        if(adress_index < 4 && (addresses[adress_index + 1] - addresses[adress_index]) != 48) 
        {
            is_working += 2;
            break;
        }
        
        if(adress_index >= 5 && (addresses[adress_index] != 0))
        {
            is_working += 4;
            break;
        }
    }

    free1 = (void *)addresses[3];
    free2 = (void *)addresses[2];
    VsaFree(free1);

    if(32 != VsaLargestChunkAvailable(vsa))
    {
        is_working += 8;
    }
    
    VsaFree(free2);
    
    if(80 != VsaLargestChunkAvailable(vsa))
    {
        is_working += 16;
    }
    
    VsaAllocate(vsa, 29);
    
    if(32 != VsaLargestChunkAvailable(vsa))
    {
        is_working += 32;
    }
    
 if(0 == is_working)
    {
         printf("VsaLargestChunkAvailable working                 SUCCESS!!!\n");
    }
    else
    {
         printf("VsaLargestChunkAvailable NOT working ERROR_CODE  = %d       FAIL!!!\n", is_working);
    
    }
    
    free(pool);
    
}

