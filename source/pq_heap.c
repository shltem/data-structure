#include <stdlib.h>/*malloc, free*/
#include <assert.h> /*assert*/
#include "heap.h"
#include "pq_heap.h"

struct priority_q
{
	bin_heap_t *heap;
};

priority_q_t *PriorityQCreate(priority_q_cmp_t func)
{
	priority_q_t *queue = NULL;
	
	assert(NULL != func);
	queue = (priority_q_t *)malloc(sizeof(priority_q_t));
	if(NULL == queue)
	{
		return NULL;
	}
	queue->heap = BinHeapCreate(func);
	if (NULL == queue->heap)
	{
		free(queue);
		return NULL;
	}
	return queue;
}

void PriorityQDestroy(priority_q_t *queue)
{
	assert(NULL != queue);
	BinHeapDestroy(queue->heap);
	queue->heap = NULL;
	free(queue);
}

int PriorityQIsEmpty(const priority_q_t *queue)
{
	assert(NULL != queue);
	return BinHeapIsEmpty(queue->heap);
}

int PriorityQEnqueue(priority_q_t *queue, void *data)
{
	int status = 0;
	assert(NULL != queue);
	status = BinHeapPush(queue->heap, data);
	
	if(1 == status)
	{
		return 1;
	}
	
	return 0;

}

void *PriorityQDequeue(priority_q_t *queue)
{	
	assert(NULL != queue);
	return BinHeapPop(queue->heap);
}

void *PriorityQErase(priority_q_t *queue, priority_q_is_match_t is_match, const void *param)
{
	void *data_to_return = NULL;
	void *data = NULL;

	assert(NULL != queue);
	assert(NULL != is_match);
	
	data_to_return = BinHeapRemove(queue->heap , is_match, (void *)param);
	if(0 == is_match(data_to_return, param))
	{
		return NULL;
	}

	return data_to_return;
}

size_t PriorityQSize(const priority_q_t *queue)
{	
	assert(NULL != queue);
	return BinHeapSize(queue->heap);
}

void *PriorityQPeek(const priority_q_t *queue)
{
	assert(NULL != queue);
	return BinHeapPeek(queue->heap);
}

void PriorityQClear(priority_q_t *queue)
{
	assert(NULL != queue);

	while(!BinHeapIsEmpty(queue->heap))
	{
		BinHeapPop(queue->heap);
	}
}

