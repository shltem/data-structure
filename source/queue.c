#include <stdlib.h>/*malloc & free*/
#include <assert.h>/*assert*/

#include "queue.h"
#include "slist.h"

struct queue
{ 
 	slist_ptr_t slist;
 	size_t size;
};
 
/*approved by Keren*/
queue_ptr_t QCreate(void)
{
	
	queue_ptr_t queue = (queue_ptr_t)malloc(sizeof(struct queue));
	if (NULL == queue)
	{
		return NULL;
	}
	queue -> slist = SListCreate();
	if (NULL == queue -> slist)
	{
		return NULL;
	}
	queue -> size = 0;
	
	return queue;
	
}

void QDestroy(queue_ptr_t queue)
{
	assert(queue);
	SListDestroy(queue -> slist);
	free(queue);
}

int QIsEmpty(const queue_ptr_t queue)
{
	assert(queue);
	
	return (0 == queue -> size);
}

q_status_t QEnQueue(queue_ptr_t queue, const void *element)
{
	assert(queue);
	assert(element);
	printf("element = %p &&&&  %d\n", element, *(int*)element);
	if(NULL == SListInsertBefore(SListEnd(queue -> slist), element))
	{
		return FAIL;
	}
	
	queue -> size += 1;
	
	return SUCCESS;
}

void QDeQueue(queue_ptr_t queue)
{
	assert(queue);
	SListRemove(SListBegin(queue -> slist));
	queue -> size -= 1;
}

void *QPeek(const queue_ptr_t queue)
{
	void *data = NULL;
	slist_iter iter = NULL;
	assert(queue);
	iter = SListBegin(queue -> slist);
	data = (void *)SListGetData(iter);
	printf("data = %p &&&&  %d\n", data, *(int*)data);
	return data;
}

size_t QGetSize(const queue_ptr_t queue)
{
	assert(queue);
	return queue -> size;	
}

void QAppend(queue_ptr_t qdest, queue_ptr_t qsrc)
{
	assert(qdest);
	assert(qsrc);
	SListAppend(qdest -> slist, qsrc -> slist);

	qdest -> size += qsrc -> size;
	qsrc -> size = 0;	
}

