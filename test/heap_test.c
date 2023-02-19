#include "heap.h"
#include <stdio.h>/*printf*/

int IsMatch(const void *data, const void *param);
int Comper(const void *key1, const void *key2);

int main()
{
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
   int num4 = 4;
    int num5 = 5;
    int num6 = 6;
    int num7 = 7;
    int num8 = 8;
    int num9 = 9;
    int num10 = 10;


    bin_heap_t *bin_heap = NULL;
    bin_heap = BinHeapCreate(Comper);
    if(NULL == bin_heap)
    {
        printf("crate fail\n");
        return -1;
    }

    printf("is empty %d \n", BinHeapIsEmpty(bin_heap));
    BinHeapPush(bin_heap, (void *)&num6);
    BinHeapPush(bin_heap, (void *)&num7);
    BinHeapPush(bin_heap, (void *)&num8);
    BinHeapPush(bin_heap, (void *)&num5);
    BinHeapPush(bin_heap, (void *)&num2);
    BinHeapPush(bin_heap, (void *)&num4);
    BinHeapPush(bin_heap, (void *)&num3);
    BinHeapPush(bin_heap, (void *)&num1);
    BinHeapPush(bin_heap, (void *)&num9);
    BinHeapPush(bin_heap, (void *)&num10);

    PrintHeap(bin_heap)    ;

    printf("peek %d\n", *(int *)BinHeapPeek(bin_heap));
    printf("is empty %d \n", BinHeapIsEmpty(bin_heap));
    printf("size %lu\n", BinHeapSize(bin_heap));
    printf("remove %d\n", *(int *)BinHeapRemove(bin_heap, IsMatch, (void *)&num3 ));

    PrintHeap(bin_heap)    ;


    while (!BinHeapIsEmpty(bin_heap))
    {
        printf("pop %d\n", *(int *)BinHeapPop(bin_heap));
        PrintHeap(bin_heap)    ;
    }



/*    printf("pop %d\n", *(int *)BinHeapPop(bin_heap));
    printf("pop %d\n", *(int *)BinHeapPop(bin_heap));
    printf("pop %d\n", *(int *)BinHeapPop(bin_heap));
    printf("pop %d\n", *(int *)BinHeapPop(bin_heap));
    printf("pop %d\n", *(int *)BinHeapPop(bin_heap));
    printf("pop %d\n", *(int *)BinHeapPop(bin_heap));

    printf("peek %d\n", *(int *)BinHeapPeek(bin_heap));
  */  printf("size %lu\n", BinHeapSize(bin_heap));

    BinHeapDestroy(bin_heap);
    return 0;
}

int Comper(const void *key1, const void *key2)
{
	return (*(int *)key1 - *(int *)key2);
}
 int IsMatch(const void *data, const void *param)
 {
    return (*(int *)data == *(int *)param);
 }
