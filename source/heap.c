#include <assert.h>/*assert*/
#include <stdlib.h>/*malloc free*/
#include "heap.h"
#include "vector.h"
#define ROOT_INDEX (0)
#define PARENT(index) ((index -1) / 2)
#define LEFT_CHILD(index) (index * 2 + 1)
#define RIGHT_CHILD(index) (index * 2 + 2)

void PrintHeap(bin_heap_t *bin_heap);

static int CmpElement(bin_heap_t *bin_heap, size_t index1, size_t index2);
static void HeapifyDown(bin_heap_t *bin_heap, size_t index);
static void HeapifyUp(bin_heap_t *bin_heap, size_t index);
static void SwapElements(void **data1, void **data2);



struct binary_heap
{
    vector_t *vector;
    cmp_func_t cmp_fun;
};

bin_heap_t *BinHeapCreate(cmp_func_t cmp_fun)
{
    bin_heap_t *bin_heap = NULL;
    
    assert(NULL != cmp_fun);
    
    bin_heap = (bin_heap_t *)malloc(sizeof(bin_heap_t));
    if(NULL == bin_heap)
    {
        return NULL;
    }
    
    bin_heap->vector = VectorCreate(100, sizeof(void *));
    if(NULL == bin_heap->vector)
    {
        free(bin_heap);
        return NULL;

    }
    
    bin_heap->cmp_fun = cmp_fun;

    return bin_heap;

}

void BinHeapDestroy(bin_heap_t *bin_heap)
{
    assert(NULL != bin_heap);

    VectorDestroy(bin_heap->vector);
   
    free(bin_heap);
}

int BinHeapIsEmpty(const bin_heap_t *bin_heap)
{
    assert(NULL != bin_heap);

    return VectorIsEmpty(bin_heap->vector);
}

size_t BinHeapSize(const bin_heap_t *bin_heap)
{
    assert(NULL != bin_heap);

    return VectorGetSize(bin_heap->vector);
}

int BinHeapPush(bin_heap_t *bin_heap, void *data)
{
    int status = SUCSESS;
    assert(NULL != bin_heap);

    status = VectorPushBack(bin_heap->vector, &data);
    if(SUCSESS != status)
    {
        return FAIL;
    }
    HeapifyUp(bin_heap, VectorGetSize(bin_heap->vector) -1);
    return SUCSESS;

}   

void *BinHeapPop(bin_heap_t *bin_heap)
{
    void *data_to_return = NULL;
    assert(NULL != bin_heap);

    SwapElements(VectorAccessAt(bin_heap->vector, 0), VectorAccessAt(bin_heap->vector, VectorGetSize(bin_heap->vector) -1));
    data_to_return = *(void **)VectorPopBack(bin_heap->vector);

    HeapifyDown(bin_heap, ROOT_INDEX);
    
    return data_to_return;

}

void *BinHeapPeek(const bin_heap_t *bin_heap)
{
    assert(NULL != bin_heap);
    return *(void **)VectorAccessAt(bin_heap->vector, ROOT_INDEX);
}

void *BinHeapRemove(bin_heap_t *bin_heap, heap_is_match_t is_match, void *param)
{
    size_t index = 0;
    size_t heap_size = 0;
    void *data_to_return = NULL;
    assert(NULL != bin_heap);
    heap_size = BinHeapSize(bin_heap);

    for(index = 0; index < heap_size -1; ++index)
    {
        if(1 == is_match(*(int **)VectorAccessAt(bin_heap->vector, index), param))
        {
            break;
        }
    }
    
    data_to_return = *(void **)VectorAccessAt(bin_heap->vector, index);
    SwapElements(VectorAccessAt(bin_heap->vector, index), VectorAccessAt(bin_heap->vector, BinHeapSize(bin_heap)-1));
    if(BinHeapSize(bin_heap) == index)
    {
        return NULL;
    }
    if((BinHeapSize(bin_heap) - 1) == index)
    {
        VectorPopBack(bin_heap->vector);
    }
    else
    {
        VectorPopBack(bin_heap->vector);
        HeapifyUp(bin_heap, index);
        HeapifyDown(bin_heap, index);
    }

    return data_to_return;
}

static void SwapElements(void **data1, void **data2)
{
	void *temp = NULL;
    temp = *data1;
    *data1 = *data2;
    *data2 = temp;

}

static void HeapifyUp(bin_heap_t *bin_heap, size_t index)
{
   
   while (0 != index)
   {
        if(0 < CmpElement(bin_heap, index, PARENT(index)))
        {
            SwapElements(VectorAccessAt(bin_heap->vector, index), VectorAccessAt(bin_heap->vector, PARENT(index)));
        }
        else
        {
            break;
        }
        index = PARENT(index);
   }

}

static void HeapifyDown(bin_heap_t *bin_heap, size_t index)
{
    size_t big_index = 0;
   
    while((index < (VectorGetSize(bin_heap->vector)) && (LEFT_CHILD(index) < (VectorGetSize(bin_heap->vector) ))))
    {
        if(RIGHT_CHILD(index) > (VectorGetSize(bin_heap->vector) -1))
        {
            big_index = LEFT_CHILD(index);
        }
        else
        {
            big_index = (0 < CmpElement(bin_heap, LEFT_CHILD(index), RIGHT_CHILD(index)) ? LEFT_CHILD(index) : RIGHT_CHILD(index)) ;
        }

        if(0 > CmpElement(bin_heap, index, big_index))
        {
            SwapElements(VectorAccessAt(bin_heap->vector, index), (VectorAccessAt(bin_heap->vector, big_index)));
        }
        else
        {
            break;
        }       
        
        index = big_index;
    }
}

static int CmpElement(bin_heap_t *bin_heap, size_t index1, size_t index2)
{
    return bin_heap->cmp_fun(*(void **)VectorAccessAt(bin_heap->vector, index1), *(void **)(VectorAccessAt(bin_heap->vector, index2)));
}

void PrintHeap(bin_heap_t *bin_heap)
{
    int index = 0 ;
    for (index =0; index<BinHeapSize(bin_heap); index++)
    {
        printf("%d : ", **(int **)VectorAccessAt(bin_heap->vector, index));
    }
       printf("\n");

    for (index =0; index<BinHeapSize(bin_heap); index++)
    {
        printf("\n %d : ", **(int **)VectorAccessAt(bin_heap->vector, index));
        if (LEFT_CHILD(index) < (VectorGetSize(bin_heap->vector) ))
        printf(" ( %d,  ", **(int **)VectorAccessAt(bin_heap->vector, LEFT_CHILD(index)  ));
    
        if(RIGHT_CHILD(index) < (VectorGetSize(bin_heap->vector) ))
        printf(" %d ) ", **(int **)VectorAccessAt(bin_heap->vector, RIGHT_CHILD(index)));
    }
       printf("\n");
}