#include <stdlib.h>/*malloc, free*/

#include <assert.h> /*assert*/
#include "sortedlist.h"
#include "priorityq.h"

struct priority_q
{
	sortedlist_t *list;
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
	queue->list = SortedListCreate(func);
	if (NULL == queue->list)
	{
		free(queue);
		return NULL;
	}
	return queue;
}

void PriorityQDestroy(priority_q_t *queue)
{
	assert(NULL != queue);
	SortedListDestroy(queue->list);
	queue->list = NULL;
	free(queue);
}
int PriorityQIsEmpty(const priority_q_t *queue)
{
	assert(NULL != queue);
	return SortedListIsEmpty(queue->list);
}
int PriorityQEnqueue(priority_q_t *queue, void *data)
{
	sortedlist_iter_t iter;
	assert(NULL != queue);
	iter = SortedListInsert(queue->list, data);
	
	if(SortedListIsSameIter(iter, SortedListEnd(queue->list)))
	{
		return 1;
	}
	
	return 0;

}
void *PriorityQDequeue(priority_q_t *queue)
{	
	assert(NULL != queue);
	return SortedListPopBack(queue->list);
}

void *PriorityQErase(priority_q_t *queue, priority_q_is_match_t is_match, const void *param)
{
	void *data_to_return = NULL;
	sortedlist_iter_t iter;

	assert(NULL != queue);
	assert(NULL != is_match);
	
	iter = SortedListFindIf(SortedListBegin(queue->list), SortedListEnd(queue->list), is_match, param);
	if(SortedListIsSameIter(iter, SortedListEnd(queue->list)))
	{
		return NULL;
	}
	data_to_return = SortedListGetData(iter);
	SortedListRemove(iter);

	return data_to_return;
}

size_t PriorityQSize(const priority_q_t *queue)
{	
	assert(NULL != queue);
	return SortedListSize(queue->list);
}

void *PriorityQPeek(const priority_q_t *queue)
{
	assert(NULL != queue);
	return SortedListGetData(SortedListIterPrev(SortedListEnd(queue->list)));
}

void PriorityQClear(priority_q_t *queue)
{
	assert(NULL != queue);

	while( 1 != SortedListIsEmpty(queue->list))
	{
		SortedListPopFront(queue->list);
	}
}

