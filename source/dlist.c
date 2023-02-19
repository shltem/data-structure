#include <assert.h>/*assert*/
#include <stdlib.h>/*malloc, free*/
#define UNUSED(x) (void)(x)
#include "dlist.h"


struct dlist_node
{
    void *data;
    dlist_node_t *next;
    dlist_node_t *prev;
};


struct dlist
{
    dlist_node_t head;
    dlist_node_t tail;
};

static int Counter(void *data, void *param);
dlist_t *DoublyListCreate(void)
{
	dlist_t *dlist = (dlist_t *)malloc(sizeof(struct dlist));
	if(NULL == dlist)
	{
		return NULL;
	}
	
	dlist->head.data = NULL;
	dlist->head.prev = NULL;
	dlist->head.next = &dlist->tail;
	dlist->tail.data = NULL;
	dlist->tail.prev = &dlist->head;
	dlist->tail.next = NULL;
	
	return dlist;
}
void DoublyListDestroy(dlist_t *dlist)
{
	dlist_node_t *current_node = NULL;
	dlist_node_t *next_node = NULL;
	assert(NULL != dlist);
	current_node = dlist->head.next;
	while(NULL != current_node->next)
	{
		next_node = current_node->next;
		current_node->data = NULL;
		free(current_node);
		current_node = next_node;
	}
	free(dlist);
}

dlist_iter_t DoublyListBegin(const dlist_t *dlist)
{
	assert(NULL != dlist);
	return (dlist->head.next);
}

dlist_iter_t DoublyListEnd(const dlist_t *dlist)
{
	assert(NULL != dlist);
	return (dlist_iter_t)&dlist->tail;
}

int DoublyListIsEmpty(const dlist_t *dlist)
{
	assert(NULL != dlist);
	return (NULL == dlist->head.next->next);
}

dlist_iter_t DoublyListInsertBefore(dlist_t *dlist, dlist_iter_t place_to_insert, void *data)
{
	dlist_node_t *new_node = NULL;
	assert(NULL != place_to_insert);
	new_node = (dlist_node_t *)malloc(sizeof(dlist_node_t));
	if(NULL == new_node)
	{
		return dlist->tail.prev;
	}
	new_node->data = data;
	new_node->next = place_to_insert;
	new_node->prev = place_to_insert->prev;
	new_node->prev->next = new_node;
	place_to_insert->prev = new_node;
	
	return new_node;
}

void DoublyListRemove(dlist_iter_t iter)
{
	dlist_node_t *next_node = NULL;
	dlist_node_t *prev_node = NULL;
	assert(NULL != iter);
	next_node = iter->next;
	prev_node = iter->prev;
	
	next_node->prev = iter->prev;
	prev_node->next = iter->next;
	iter->data = NULL;
	free(iter);
}


dlist_iter_t DoublyListIterNext(dlist_iter_t iter)
{
	assert(NULL != iter);
	return(iter->next);
}

dlist_iter_t DoublyListIterPrev(dlist_iter_t iter)
{
	assert(NULL != iter);
	return(iter->prev);
}

size_t DoublyListSize(const dlist_t *dlist)
{
	size_t counter = 0;
	assert(NULL != dlist);

	DoublyListForEach(dlist->head.next, (dlist_iter_t)&dlist->tail, Counter, (void *)&counter);
	return counter;
	
}

void *DoublyListGetData(const dlist_iter_t iter)
{
	assert(NULL != iter);
	return iter->data;

}
void DoublyListSetData(const dlist_iter_t iter, void *data)
{
	assert(NULL != iter);
	iter->data = data;
}

int DoublyListIsSameIter(dlist_iter_t iter_one, dlist_iter_t iter_two)
{
	return (iter_one == iter_two);
}

dlist_iter_t DoublyListPushFront(dlist_t *dlist , void *data)
{
	dlist_node_t *new_node = NULL;
	
	assert(NULL != dlist);

	new_node = (dlist_node_t *)malloc(sizeof(dlist_node_t));
	if(NULL == new_node)
	{
		return NULL;
	}
	new_node->data = data;
	new_node->next= dlist->head.next;
	dlist->head.next->prev = new_node;
	dlist->head.next = new_node;
	new_node->prev = &dlist->head;
	
	return (dlist_iter_t)new_node;
}

dlist_iter_t DoublyListPushBack(dlist_t *dlist , void *data)
{
	dlist_node_t *new_node = NULL;
	
	assert(NULL != dlist);

	new_node = (dlist_node_t *)malloc(sizeof(dlist_node_t));
	if(NULL == new_node)
	{
		return NULL;
	}
	dlist->tail.prev->next = new_node;
	new_node->data = data;
	new_node->next= &dlist->tail;
	new_node->prev = dlist->tail.prev;
	dlist->tail.prev = new_node;
	
	return new_node;
}

void *DoublyListPopFront(dlist_t *dlist)
{
	dlist_node_t *to_remove = NULL;
	void *data = NULL;
	assert(NULL != dlist);
	to_remove = dlist->head.next;
	
	data = dlist->head.next->data;
	dlist->head.next = dlist->head.next->next;
	dlist->head.next->prev = &dlist->head;
	free(to_remove);
	
	return data;
}

void *DoublyListPopBack(dlist_t *dlist)
{
	dlist_node_t *to_remove = NULL;
	void *data = NULL;
	assert(NULL != dlist);

	to_remove = dlist->tail.prev;
	data = dlist->tail.prev->data;
	dlist->tail.prev = dlist->tail.prev->prev;
	dlist->tail.prev->next = &dlist->tail;
	free(to_remove);
	
	return data;
}

dlist_iter_t DoublyListFind(dlist_iter_t from, dlist_iter_t to, is_match_t func, const void *param)
{
	dlist_iter_t runner = NULL;
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != func);		
	
	runner = from;
	
	for(runner = from; to != runner; runner = runner->next)
	{
		if(1 == func(runner->data, param))
		{
			break;
		}
	}
	return runner;

}

size_t DoublyListMultiFind(dlist_iter_t from, dlist_iter_t to, is_match_t func, const void *param, dlist_t *output_list )
{
	size_t counter = 0;
	dlist_iter_t runner = NULL;
	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != func);		

	runner = from;
	while(to != runner)
	{
		if(1 == func(runner->data, param))
		{
			DoublyListPushFront(output_list, runner->data);
			++counter;			
		}
		runner = runner->next;
	}

	return counter;	
}

int DoublyListForEach(dlist_iter_t from, dlist_iter_t to, dlist_action_t action_func, void *param)
{
	int status = 0;
	dlist_iter_t runner = NULL;

	assert(NULL != from);
	assert(NULL != to);
	assert(NULL != action_func);		
	
	runner = from;

	while(to != runner)
	{
		status = action_func(runner->data, param);
		if (0 != status)
		{
			break;
		}
		runner = runner->next;
	}

	return status;
}

dlist_iter_t DoublyListSplice(dlist_iter_t iter, dlist_iter_t from, dlist_iter_t to)
{

	dlist_node_t *old_from_prev = NULL;
	
	assert(NULL != iter);
	assert(NULL != from);
	assert(NULL != to);		
	
	old_from_prev = from->prev;

	from->prev->next = to;
	iter->prev->next = from;
	to->prev->next = iter;
	
	from->prev = iter->prev;
	iter->prev = to->prev;	
	to->prev = old_from_prev;	
	
	return from;
}

static int Counter(void *data, void *param)
{
	++*(size_t*)param;
	UNUSED(data);
	return 0;
}


