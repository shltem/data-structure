#include <assert.h>/*assert*/
#include <stdlib.h>/*malloc, free*/
#include "sortedlist.h"
#include "dlist.h"

struct sorted_list
{
    dlist_t *sorted_list;
    compare_t compare;
};



static int IsEndAndIterWhileSmall(sortedlist_iter_t *iter1, sortedlist_iter_t iter2, sortedlist_iter_t end_list, sortedlist_t *sortedlist);
static sortedlist_iter_t ConvereDoublyToSortedIter(dlist_iter_t dlist_iter,
												   sortedlist_t *sorted_list);
static void IsItersFromSameList(sortedlist_iter_t iter1, sortedlist_iter_t iter2);
sortedlist_t *SortedListCreate(compare_t func)
{
	sortedlist_t *sorted_list = NULL;

	assert(NULL != func);

	sorted_list = (sortedlist_t *)(malloc(sizeof(sortedlist_t)));
	if(NULL == sorted_list)
	{
		return NULL;
	}

	sorted_list->sorted_list = DoublyListCreate();
	if(NULL == sorted_list->sorted_list)
	{
		free(sorted_list);
		return NULL;
	}
	sorted_list->compare = func;
	
	return sorted_list;
}

void SortedListDestroy(sortedlist_t *sortedlist)
{
	assert(NULL != sortedlist);
	
	DoublyListDestroy(sortedlist->sorted_list);
	sortedlist->sorted_list = NULL;
	
	free(sortedlist);
}

sortedlist_iter_t SortedListBegin(sortedlist_t *sortedlist)
{	
	assert(NULL != sortedlist);
	
	return ConvereDoublyToSortedIter(DoublyListBegin(sortedlist->sorted_list), 
									 sortedlist);
}

sortedlist_iter_t SortedListEnd(sortedlist_t *sortedlist)
{
	assert(NULL != sortedlist);
	return ConvereDoublyToSortedIter(DoublyListEnd(sortedlist->sorted_list),
									 sortedlist);
}

int SortedListIsEmpty(const sortedlist_t *list)
{
		assert(NULL != list);
		return DoublyListIsEmpty(list->sorted_list);
}

sortedlist_iter_t SortedListInsert(sortedlist_t *sortedlist, void *data)
{
	sortedlist_iter_t end_of_list;
	sortedlist_iter_t runner;
	assert(NULL != sortedlist);
	
	runner = SortedListBegin(sortedlist);
	end_of_list = SortedListEnd(sortedlist);
	
	while (!SortedListIsSameIter(runner, end_of_list))
	{
		if (0 < (sortedlist->compare(SortedListGetData(runner), data)))
		{	
			break ;		
		}
		runner = SortedListIterNext(runner);
	}
	
	DoublyListInsertBefore(sortedlist->sorted_list, runner.iter, data);
		
	return SortedListIterPrev(runner);
}

sortedlist_iter_t SortedListRemove(sortedlist_iter_t iter)
{
	sortedlist_iter_t iter_to_return;
	
	assert(NULL != iter.iter);
	
	iter_to_return = SortedListIterNext(iter);
	DoublyListRemove(iter.iter);
	
	return iter_to_return;
}

void *SortedListGetData(sortedlist_iter_t iter)
{
	assert(NULL != iter.iter);
	return DoublyListGetData(iter.iter);
}

sortedlist_iter_t SortedListIterNext(sortedlist_iter_t iter)
{
	assert(NULL != iter.iter);
	return ConvereDoublyToSortedIter(DoublyListIterNext(iter.iter), 
									iter.sortedlist);
}

sortedlist_iter_t SortedListIterPrev(sortedlist_iter_t iter)
{
	assert(NULL != iter.iter);
	return ConvereDoublyToSortedIter(DoublyListIterPrev(iter.iter), iter.sortedlist);
}

size_t SortedListSize(sortedlist_t *sortedlist)
{

	return DoublyListSize(sortedlist->sorted_list);
}

int SortedListIsSameIter(sortedlist_iter_t iter_one, sortedlist_iter_t iter_two)
{
	assert(NULL != iter_one.iter);
	assert(NULL != iter_two.iter);
	return (iter_one.iter) == (iter_two.iter);

}

void *SortedListPopFront(sortedlist_t *sortedlist)
{
	assert(NULL != sortedlist);
	return DoublyListPopFront(sortedlist->sorted_list);
}

void *SortedListPopBack(sortedlist_t *sortedlist)
{
	assert(NULL != sortedlist);
	return DoublyListPopBack(sortedlist->sorted_list);
	
}

int SortedListForEach(sortedlist_iter_t from, sortedlist_iter_t to, 
					  sortedlist_action_t action_func, void *param)
{
	int status = 0;
	#ifndef NDEBUG 
	IsItersFromSameList(from, to);
	#endif
	assert(NULL != from.iter);
	assert(NULL != to.iter);
	assert(NULL != action_func);

	status = DoublyListForEach(from.iter, to.iter, action_func, param);

	return status;
}

sortedlist_iter_t SortedListFindIf(sortedlist_iter_t from, sortedlist_iter_t to, 
is_match_t func, const void *param)
{
	sortedlist_iter_t iter;

	assert(NULL != from.iter);
	assert(NULL != to.iter);
	#ifndef NDEBUG 
	IsItersFromSameList(from, to);
	#endif
	
	iter.iter = DoublyListFind(from.iter, to.iter, func, param);
	
	return iter;
}

sortedlist_iter_t SortedListFind(const sortedlist_t *sortedlist, sortedlist_iter_t from, sortedlist_iter_t to, const void *data_to_find)
{
	sortedlist_iter_t runner;
	assert(NULL != from.iter);
	assert(NULL != to.iter);
	#ifndef NDEBUG 
	IsItersFromSameList(from, to);
	#endif

	runner = from;
	while(!SortedListIsSameIter(runner, to))
	{
		if(0 == sortedlist->compare(SortedListGetData(runner), data_to_find))
		{
			return runner;
		}
		runner = SortedListIterNext(runner);
	}

	return to;

}

void SortedListMerge(sortedlist_t *sorted_list_one, sortedlist_t *sorted_list_two)
{
	sortedlist_iter_t iter;
	sortedlist_iter_t from;
	sortedlist_iter_t to;
	
	sortedlist_iter_t sorted_list_1end = SortedListEnd(sorted_list_one);
	sortedlist_iter_t sorted_list_2end = SortedListEnd(sorted_list_two);
	
	assert(NULL != sorted_list_one->sorted_list);
	assert(NULL != sorted_list_two->sorted_list);
	
	iter = SortedListBegin(sorted_list_one);
	from = SortedListBegin(sorted_list_two);
	to = from;
	
	while (1)
	{
		if( 1 == IsEndAndIterWhileSmall(&iter, from, sorted_list_1end, sorted_list_one))
		{
			DoublyListSplice((iter.iter) ,from.iter ,sorted_list_2end.iter);
			break;
		}
		if( 1 == IsEndAndIterWhileSmall(&to, iter, sorted_list_2end, sorted_list_one))
		{
			DoublyListSplice(iter.iter ,from.iter ,sorted_list_2end.iter);
			break;
		}
		DoublyListSplice(iter.iter ,from.iter ,to.iter);
		from = to;
	}
	
}
static int IsEndAndIterWhileSmall(sortedlist_iter_t *runner, sortedlist_iter_t stop_iter,  sortedlist_iter_t end_list, sortedlist_t *sortedlist)
{
	for (;runner->iter != end_list.iter; *runner = SortedListIterNext(*runner))
	{
		if(0 < sortedlist->compare(SortedListGetData(*runner), 
											SortedListGetData(stop_iter)))
		{
			return 0;
		}
	}
	return 1;
}	


static sortedlist_iter_t ConvereDoublyToSortedIter(dlist_iter_t dlist_iter,
												   sortedlist_t *sorted_list)
{
	sortedlist_iter_t sorted_iter = {0};
	sorted_iter.iter = dlist_iter;
	#ifndef NDEBUG 
	sorted_iter.sortedlist = sorted_list;
	#endif
	return sorted_iter;
}
static void IsItersFromSameList(sortedlist_iter_t iter1, sortedlist_iter_t iter2)
{
	#ifndef NDEBUG 
	assert(iter1.sortedlist == iter2.sortedlist);
	#endif
}



