#include "recorsive_sort.h"

int Comper(const void *data1, const void *data2)
{
	return (*(int *)data1 - *(int *)data2);
}

int main()
{
	int array[ARR_SIZE] = {0};
	InitArray(array, ARR_SIZE);
	PrintArraty(array, ARR_SIZE, 0, ARR_SIZE);
	QuickSort(array,ARR_SIZE, sizeof(int), Comper);
	/*MergeSort(array, ARR_SIZE);*/
	PrintArraty(array, ARR_SIZE, 0, ARR_SIZE);

	return 0;

}
