#include <stdlib.h>/*malloc & free*/
#include <string.h>/*memcpy*/
#include <assert.h>/*assert*/
#include <stdio.h>
#include "../include/slist.h"

/*aproved by omri*/
struct slist
{
	slist_node_ptr_t head;
	slist_node_ptr_t tail;
};

struct slist_node
{
	void *data;
	slist_node_ptr_t next;
};


slist_ptr_t SListCreate(void)
{

	slist_node_ptr_t dummy_node = NULL;
	slist_ptr_t slist = (slist_ptr_t)malloc(sizeof(struct slist));
	if (NULL == slist)
	{
		return NULL;
	}
	dummy_node = (slist_node_ptr_t)malloc(sizeof(struct slist_node));
	if (NULL == dummy_node)
	{
		free(slist);
		return NULL;
	}

	slist -> head = dummy_node;
	slist -> tail = dummy_node;
	dummy_node -> next = NULL;
	dummy_node -> data = slist;
	
	return slist;
}

void SListDestroy(slist_ptr_t list)
{
	slist_node_ptr_t next_ptr = list -> head;

	assert(list);
	
	while(NULL != next_ptr)
	{
		next_ptr = next_ptr -> next;
		free(list -> head);
		list -> head = next_ptr;
	}
	
	free(list);
}

slist_iter SListFind(slist_iter from, slist_iter to, is_match_t func, void *data)
{
	
	slist_node_ptr_t runner = from -> next;
	
	assert(from);
	assert(to);
	assert(func);
	assert(data);	
	
			
	while(to != runner)
	{
        if(func(data, runner -> data))
        {
        	return runner;
        }
		runner = runner -> next;
	}
	
	return to;
}

void SListRemove(slist_iter iter)
{
	slist_node_ptr_t next_node = NULL;
	assert(iter);	

	if(NULL != iter -> next)
	{
		next_node = iter -> next;
		iter -> data = next_node -> data;
		iter -> next = next_node -> next;
		free(next_node);
	}
}

slist_iter SListInsertBefore(slist_iter place_to_insert, const void *data)
{

	slist_node_ptr_t cpy_node = (slist_node_ptr_t)malloc(sizeof(struct
						 	slist_node));
	assert(place_to_insert);
	assert(data);
	
	
	cpy_node -> data = place_to_insert -> data;
 						
	cpy_node -> next = place_to_insert -> next;							
	
	place_to_insert -> data = (void *)data;
							
	if(NULL == place_to_insert -> next)
	{
		((slist_ptr_t)cpy_node -> data) -> tail = cpy_node;
	}
	
	place_to_insert -> next = cpy_node;							

	return place_to_insert;
}

size_t SListCount(const slist_ptr_t list)
{
	
	size_t counter = 0;
	
	slist_iter runner = list -> head;
	
	assert(list);
	
	while(NULL != runner -> next)
	{
	
		++counter;
		runner = runner -> next;
	}
	
	return counter;
	
}

const void *SListGetData(const slist_iter iter)
{
	assert(iter);

	return iter -> data;

}

void SListSetData(slist_iter iter, void *data)
{
	assert(iter);
	assert(data);
	if (NULL != (iter -> next))
	{
		iter -> data = data;
	}
}

slist_iter SListIterNext(slist_iter iter)
{

	assert(iter);
	return iter -> next;
}

slist_iter SListBegin(const slist_ptr_t list)
{
	assert(list);
	
	return list -> head;
	
}

slist_iter SListEnd(const slist_ptr_t list)
{
	assert(list);
	
	return list -> tail;
	
}

int SListIsIterEqual(slist_iter iter_one, slist_iter iter_two)
{
	assert(iter_one);
	assert(iter_two);

	return (iter_one == iter_two);
}

int SListForEach(slist_iter from, slist_iter to, slist_action_t action_func, void *param)
{
	int flag = 0;
	printf("SListForEach");
	assert(to);
	assert(action_func);
	
	while(to != from)
	{
		flag = action_func(from -> data, param);
        if(0 != flag)
        {
			return 1;
        }
		from = from -> next;
	}

	return 0;
}
	
void SListAppend(slist_ptr_t slist_dest, slist_ptr_t slist_src)
{
	slist_node_ptr_t dummy_source = NULL;
	slist_node_ptr_t dummy_dest = NULL;
	
	assert(slist_dest);
	assert(slist_src);
	
	dummy_source = (slist_node_ptr_t)malloc(sizeof(struct slist_node));
	
	dummy_dest = slist_dest -> tail;
		
	slist_dest -> tail -> next = slist_src -> head;
	slist_src -> tail -> data = slist_dest;
	
	SListRemove(dummy_dest);

	slist_src -> head = dummy_source;
	slist_src -> tail = dummy_source;
	
	dummy_source -> next = NULL;
	dummy_source -> data = slist_src;
}
