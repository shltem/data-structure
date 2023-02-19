#include <stdio.h>/*printf*/
#include "../include/vector.h"

int main()
{
	int num10 = 15;
	int num11 = 14;
	int num12 = 13;
	int num13 = 12;
	void *num2 = 0;
	vector_ptr_t vector_pointer = vectorCreate(5, 4);
	printf("%lu\n", (size_t)vectorGetCapacity(vector_pointer));
	printf("%lu\n", (size_t)vectorGetSize(vector_pointer));
	printf("%d\n",vectorIsEmpty(vector_pointer));
	printf("%lu\n", (size_t)vectorGetSize(vector_pointer));
	vectorPush(vector_pointer, &num10);
	printf("%lu\n", vectorGetSize(vector_pointer));
	vectorPush(vector_pointer, &num11);
	vectorPush(vector_pointer, &num12);
	vectorPush(vector_pointer, &num13);
	vectorPush(vector_pointer, &num10);
	printf("%lu\n", (size_t)vectorGetSize(vector_pointer));
	num2 = vectorPop(vector_pointer);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = vectorPop(vector_pointer);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = vectorPop(vector_pointer);
	printf("/n**%d**\n",*(int *)(num2));
	printf("%d\n",vectorIsEmpty(vector_pointer));
	num2 = vectorPop(vector_pointer);
	printf("%lu\n", (size_t)vectorGetSize(vector_pointer));
	num2 = 0;
	num2 = vectorPeek(vector_pointer);
	printf("/n**%d**\n",*(int *)(num2));
	vectorDestroy(vector_pointer);
	
	
	return 0;
}
