#include "hasht.h"
#include <assert.h>/*assert*/
#include <stddef.h>/*size_t*/
#include <stdlib.h>/*malloc free*/

static void HashtDestroyForNumOfLists(hasht_t *table, size_t num_of_lists);

hasht_t *HashtCreate(size_t expected_cacpacity, hash_cmp_t cmp_func, hash_func_t hash_func)
{
	size_t index = 0;
	hasht_t *table = NULL;
	assert(NULL != cmp_func);
	
	table = (hasht_t *)malloc(sizeof(hasht_t));
	if (NULL == table)
	{
		return NULL;
	}
	table->table = (dlist_t **)malloc(sizeof(dlist_t *) * expected_cacpacity);
	if (NULL == table->table)
	{
		free(table);
		return NULL;
	}
	for (index = 0; index < expected_cacpacity; ++index)
	{
		table->table[index] = DoublyListCreate();
		if (NULL == table->table[index])
		{
			HashtDestroyForNumOfLists(table, index);
			return NULL;
		}
	}

	table->hash_func = hash_func;
	table->cmp_func = cmp_func;
	table->expected_cacpacity = expected_cacpacity;

	return table;
}

void HashtDestroy(hasht_t *table)
{
	assert(NULL != table);

	HashtDestroyForNumOfLists(table, table->expected_cacpacity);
}

static void HashtDestroyForNumOfLists(hasht_t *table, size_t num_of_lists)
{
	size_t index = 0;
	
	assert(NULL != table);

	for (index = 0; index < num_of_lists; ++index)
	{
		DoublyListDestroy(table->table[index]);
	}
	
	free(table->table);
	
	free(table);
}

int HashtIsEmpty(const hasht_t *table)
{
	size_t index = 0;

	assert(NULL != table);

	for (index = 0; index < table->expected_cacpacity; ++index)
	{
		if (!DoublyListIsEmpty(table->table[index]))
		{
			return 0;
		}
	}
	return 1;
}

size_t HashtSize(const hasht_t *table)
{
	size_t index = 0;
	size_t counter = 0;
	
	assert(NULL != table);

	for (index = 0; index < table->expected_cacpacity; ++index)
	{
		counter += DoublyListSize(table->table[index]);
	}
	return counter;
}

int HashtForEach(hasht_t *table, action_func_t action_func, void *param)
{
	int status = 0;
	size_t index = 0;
	dlist_t *list = NULL;
	
	assert(NULL != table);

	for (index = 0; index < table->expected_cacpacity; ++index)
	{
		list = table->table[index];
		if (!DoublyListIsEmpty(list))
		{
			status = DoublyListForEach(DoublyListBegin(list),
									   DoublyListEnd(list), action_func, param);
			if (0 != status)
			{
				return -1;
			}
		}
	}

	return 0;
}

int HashtInsert(hasht_t *table, void *data)
{
	int index = 0;
	dlist_iter_t iter = NULL;
	
	assert(NULL != table);

	index = table->hash_func(data);
	iter = DoublyListPushBack(table->table[index], data);
	if (DoublyListIsSameIter(iter, DoublyListEnd(table->table[index])))
	{
		return 1;
	}

	return 0;
}

void *HashtFind(const hasht_t *table, const void *key)
{
	int index = 0;
	dlist_iter_t iter = NULL;
	void *data = NULL;

	assert(NULL != table);

	index = table->hash_func(key);
	iter = (DoublyListFind(DoublyListBegin(table->table[index]),
						   DoublyListEnd(table->table[index]),
						   table->cmp_func, key));
	if(DoublyListIsSameIter(iter, DoublyListEnd(table->table[index])))
	{
		return NULL;
	}
	data = DoublyListGetData(iter);
	DoublyListPushFront(table->table[index], data);
	DoublyListRemove(iter);

	return data;
}

void HashtRemove(hasht_t *table, void *key)
{
	int index = 0;
	
	assert(NULL != table);

	index = table->hash_func(key);
	DoublyListRemove(DoublyListFind(DoublyListBegin(table->table[index]),
									DoublyListEnd(table->table[index]),
									table->cmp_func, key));
}
