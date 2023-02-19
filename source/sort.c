#include <stddef.h>/*size_t*/
#include <stdio.h>/*printf*/
#include <time.h>/*time_t*/

#define MIN(a, b) (a < b ? a : b)
#define ARR_SIZE 5000

static void Swap(int *num1, int *num2);
static void PrintArraty(int array[]);
static void InitArraty(int array[]);
void SelectionSort(int array[]);
void InsertionSort(int array[]);

void BubbleSort(int array[])
{

	size_t i = 0;
	size_t j = 0;
	
	for(i = 0; i < ARR_SIZE - 1; ++i)
	{
		for(j = 0; j < ARR_SIZE - i - 1; ++j)
		{
			if((array[j]) > (array[j+1]))
			{
				Swap(array + j, array + j +1);
			}
		}
	}
}

void SelectionSort(int array[])
{
	size_t i = 0;
	size_t j = 0;
	int min_index = 0;

	for(i = 0; i < ARR_SIZE; ++i)
	{			
		min_index = i;
		
		for(j = i + 1; j < ARR_SIZE; ++j)
		{
			if(array[j] < array[min_index])
			{
				min_index = j;
			}
		}
		
		Swap(array + i, array + min_index);
	}
}

void InsertionSort(int array[])
{
	size_t i = 0;
	size_t j = 0;

	for(i = 0; i < ARR_SIZE - 1; ++i)
	{			
		j = i;
		
		while(0 <= j && array[j] > array[j + 1])
		{
			Swap(array + j, array + j+1);
			--j;
		}
	}	
}

static void Swap(int *num1, int *num2)
{
	int temp = 0;

	temp = *num1;		
	*num1 = *num2;
	*num2 = temp;
}

static void PrintArraty(int array[])
{
	int index = 0;
	
	for(index = 0; index < ARR_SIZE; ++index)
	{
		printf("%d\n", array[index]);
	}
}

static void InitArraty(int array[])
{
	int index = 0;
	
	for(index = 0; index < ARR_SIZE; ++index)
	{
		array[index] = random();
	}
}

int Compare(const void *num1, const void *num2)
{
	return *(int *)num1 - *(int *)num2;
}


int main()
{
	int array[ARR_SIZE] = {0};
	int index = 0;
	clock_t time1;
	clock_t time2;
	InitArraty(array);
	/*PrintArraty(array);*/
	time1 = clock();
	/*BubbleSort(array);*/
	/*InsertionSort(array);*/
	/*SelectionSort(array);*/
	qsort(array, ARR_SIZE, sizeof(int), Compare );
	time2 = clock();
	printf("time : %f\n", (double)time2 -time1);
	printf("***\n***\n***\n");
	
/*	PrintArraty(array);*/
	
	
	
	return 0;
}
