#include "../include/slist.h"
#include <stdio.h>

/*int (*slist_action_t)(void *data, void *param);*/
int Print(void *data, void *param)
{
	printf("Print %d\n", (*(int*)data));
	return 0;
}

int main()
{
	
	size_t index = 0;
	slist_ptr_t slist2 = NULL;
	int num22 = 9;
	int num23 = 8;
	int num24 = 7;
	int num25 = 6;
	int num26 = 5;
	int num27 = 4;
	int num28 = 3;
	int num29 = 2;
	int num30 = 1;
    slist_ptr_t slist = SListCreate();
	slist_iter iter = SListBegin(slist);
	slist_iter iter2 = NULL;
    SListInsertBefore(iter, &num26);
    iter = SListIterNext(iter);
    SListInsertBefore(iter, &num27);
        iter = SListIterNext(iter);
       SListInsertBefore(iter, &num28);
      printf("%p  %p %p \n", (void *)SListBegin(slist), (void *)SListIterNext(iter), (void *)SListEnd(slist));
       		SListForEach(SListBegin(slist), SListEnd(slist), Print, NULL) ;
       		
	
	printf("%d\n", (*(int*)SListGetData(iter)));
	printf("%lu\n",SListCount(slist));
	slist2 = SListCreate();
	 iter2 = SListBegin(slist2);
    SListInsertBefore(iter2, &num25);
    SListInsertBefore(iter2, &num24);
        SListInsertBefore(iter2, &num23);

	SListAppend(slist2, slist);
	printf("%lu\n",SListCount(slist2));
	SListDestroy(slist2);
	SListDestroy(slist);
	return 0;
}
