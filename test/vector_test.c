#include <stdio.h>/*printf*/

#include "../include/vector.h"





int main()
{
	
	int num1 = 30;
	int num_2 = 29;
	int num3 = 28;
	int num4 = 27;
	int num5 = 26;
	int num6 = 25;
	int num7 = 24;
	int num8 = 23;
	int num9 = 22;
	int num10 = 21;
	int num11 = 20;
	int num12 = 19;
	int num13 = 18;
	int num14 = 17;
	int num15 = 16;
	int num16 = 15;
	int num17 = 14;
	int num18 = 13;
	int num19 = 12;
	int num20 = 11;
	int num21 = 10;
	int num22 = 9;
	int num23 = 8;
	int num24 = 7;
	int num25 = 6;
	int num26 = 5;
	int num27 = 4;
	int num28 = 3;
	int num29 = 2;
	int num30 = 1;
	void *num2 = 0;

	vector_ptr_t vector = VectorCreate(20 ,4);
	
	VectorPushBack(vector, &num1);
	VectorPushBack(vector, &num_2);
	VectorPushBack(vector, &num3);
	VectorPushBack(vector, &num4);
	VectorPushBack(vector, &num5);
	VectorPushBack(vector, &num6);
	VectorPushBack(vector, &num7);
	VectorPushBack(vector, &num8);
	VectorPushBack(vector, &num9);
	VectorPushBack(vector, &num10);
	VectorPushBack(vector, &num11);
	VectorPushBack(vector, &num12);
	VectorPushBack(vector, &num13);
	VectorPushBack(vector, &num14);
	VectorPushBack(vector, &num15);
	VectorPushBack(vector, &num16);
	VectorPushBack(vector, &num17);
	VectorPushBack(vector, &num18);
	VectorPushBack(vector, &num19);
	VectorPushBack(vector, &num20);
	VectorPushBack(vector, &num21);
	VectorPushBack(vector, &num22);
	VectorPushBack(vector, &num23);
	VectorPushBack(vector, &num24);
	VectorPushBack(vector, &num25);
	VectorPushBack(vector, &num26);
	VectorPushBack(vector, &num27);
	VectorPushBack(vector, &num28);
	VectorPushBack(vector, &num29);
	VectorPushBack(vector, &num30);
	printf("%d\n", VectorIsEmpty(vector));

	printf("%lu\n", VectorGetCapacity(vector));
	printf("%lu\n", VectorGetSize(vector));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));
	num2 = VectorPopBack(vector);
	printf("/n**%d**\n",*(int *)(num2));

	printf("%lu\n", VectorGetSize(vector));
	printf("%lu\n", VectorGetCapacity(vector));
	printf("%d\n", VectorIsEmpty(vector));
	VectorDestroy(vector);
	return 0;
}
