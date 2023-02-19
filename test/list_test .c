/*============================LIBRARIES && MACROS ============================*/
#include <stdio.h> /* printf */
#include <stdlib.h> /* malloc, free */

#include "../include/slist.h"

/*========================== FUNCTION DECLARATION ============================*/
int int_match(const void *data, const void *param);
int AddToNum(void* ptr, void* param);
void TestLL();
void TestAll();
/*============================ FUNCTION DEFINITION ===========================*/
int main()
{
	TestAll();

	return (0);
}


int int_match(const void *data, const void *param)
{
	int data_int = *(int *)data;
	int cmp =  *(int *)param;
	
	return (data_int == cmp);
}

int AddToNum(void* ptr, void* param)
{
    *(int*)ptr += *(int*)param;
    return (0);
}


void TestAll()
{
	int arr[100];
	int i = 0;

	slist_ptr_t list = SListCreate();
	slist_iter iter_start = SListBegin(list);
	slist_iter iter_end;
	slist_iter iter_is_found;
	int to_add = 10;
	void *adding = &to_add;
	int status = 1;
	int num1;
	int num2;
	int tester = 50;

	printf("created list\n");
	status = (SListCount(list ) == 0);
	if(1 != status)
	{
		printf("SListCount failed!\n");
		printf("SListCount output %ld\n",SListCount(list ));
		return;

	}

	printf("filling list....\n");
	for(i = 0;i < 100;++i)
	{
		arr[i] = i;
		iter_start = SListInsertBefore(iter_start, (void *)&arr[i]);
	}
	status = (SListCount(list ) == 100);
	if(1 != status)
	{
		printf("filling list failed! using SListInsertBefore\n");
		return;
	}

	num1 = SListCount(list);
	SListRemove(iter_start);
	num2 = SListCount(list);
	status = (num1 == (num2 + 1))? 1:0;
	if(1 != status)
	{
		printf("SListRemove failed!\n");
		return;
	}

	iter_end = SListEnd(list);
	SListSetData(iter_start, (void *)&tester);
	num2 = *(int *)SListGetData(iter_start);
	status = (num2 == 50)? 1 : 0;
	if(1 != status)
	{
		printf("SListSetData failed! \n");
		printf("value : %d\n",num2);
		printf("nedded to be: %d\n",arr[50]);
		return;
	}
	status = 0 == SListIsIterEqual(iter_start,iter_end)? 1 : 0;
	if(1 != status)
	{
		printf("SListIsIterEqual failed! \n");
		printf("needed to be true but got false\n");
		return;
	}

	to_add = *(int *)SListGetData(iter_start);
	iter_is_found = SListFind(SListBegin(list),SListEnd(list),int_match,adding);
	status = (to_add == *(int *)SListGetData(iter_is_found))? 1 : 0;
	if(1 != status)
	{
		printf("SListFind failed! \n");
		return;
	}

	iter_start = SListBegin(list);
	num1 = *(int *)SListGetData(iter_start);
	SListForEach(iter_start,iter_end,AddToNum,adding);
	num2 = *(int *)SListGetData(iter_start);
	status = (num1 + to_add == num2 )? 1 : 0;
	if(1 != status)
	{
		printf("SListForEach failed! \n");
		return;
	}

	printf("destroying the list...\n");
	SListDestroy(list);

	printf("SUCCESS YOU DID IT!!\n");
}









void TestLL()
{
	int arr[100];
	int i = 0;

	slist_ptr_t list = SListCreate();
	slist_iter iter_start = SListBegin(list);
	slist_iter iter_end;
	slist_iter iter_is_found;
	int first_val;
	int last_val;
	int to_add = 10;
	void *adding = &to_add;
	int tester = 50;

	printf("created list\n");
	printf("is empty : %s\n",(0 == SListCount(list))? "yes":"no");
	printf("element count : %ld\n\n",SListCount(list));

	printf("filling list....\n");
	
	for(i = 0;i < 100;++i)
	{
		arr[i] = i;
		iter_start = SListInsertBefore(iter_start, (void *)&arr[i]);
	}
	
	iter_end = SListEnd(list);
	printf("list filled!\n");
	printf("is empty : %s\n\n",(0 == SListCount(list))? "yes":"no");
	


	printf("element count before remove: %ld\n",SListCount(list));
	SListRemove(iter_start);
	printf("element count after remove: %ld\n\n",SListCount(list));



	first_val = *(int *)SListGetData(iter_start);
	last_val  = *(int *)SListGetData(iter_end);
	printf("does filling went good: %s\n\n",(0 == last_val && 98 == first_val)?\
	 "yes":"no");



	printf("a node value before changing: %d\n",*(int *)SListGetData(iter_start));
	SListSetData(iter_start, (void *)&tester);
	printf("a node value after changing: %d\n\n",*(int *)SListGetData(iter_start));



	printf("is iter begin and last is same: %s\n\n",(0==SListIsIterEqual(iter_start,iter_end))?"yes":"no");



	iter_start = SListBegin(list);
	printf("a node value before changing: %d\n",*(int *)SListGetData(iter_start));
	SListForEach(iter_start,iter_end,AddToNum,adding);
	printf("a node value after changing: %d\n\n",*(int *)SListGetData(iter_start));

	to_add = *(int *)SListGetData(iter_start);

	iter_is_found = SListFind(SListBegin(list),SListEnd(list),int_match,adding);
	printf("looked for: %d found: %d\n\n",to_add,*(int *)SListGetData(iter_is_found));


	printf("destroying the list...\n");
	SListDestroy(list);
	printf("list got destroyed : %s\n",(NULL==SListGetData(iter_is_found))? "yes":"no");
}




