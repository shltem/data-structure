#include <stdio.h>/*printf*/

#include "binary_tree.h"

int Copmare(const void *data1, const void *data2);

int main()
{
	binary_tree_t *binary_tree = NULL;
	binary_tree_iterator_t iter = NULL;
	binary_tree_iterator_t iter_prev = NULL;
	int is_empty_flag = 5;
	int num = 19;
	int array[14]= {15, 10, 20, 24, 7 , 17 ,13, 9, 16, 19, 30, 26, 25 , 11};
	int index = 0;
	binary_tree = BinaryTreeCreate(Copmare);
	if(binary_tree != NULL)
	{
		printf("BinaryTreeCreat         V\n");
	}
	else
	{
		printf("BinaryTreeCreat         X\n");
	}
	is_empty_flag = BinaryTreeIsEmpty(binary_tree);
	
	for(index = 0; index < 14 ; ++ index)
	{
		BinaryTreeInsert(binary_tree ,(void *)&array[index]);
	}
	iter = BinaryTreeBegin(binary_tree);
	if (7 == *(int *)BinaryTreeGetData(iter))
	{
		printf("BinaryTreeBegin         V\n");
	}
	else
	{
		printf("BinaryTreeBegin         X\n");
	}

	iter = BinaryTreeBegin(binary_tree);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	if (11 == *(int *)BinaryTreeGetData(iter))
	{
		printf("BinaryTreeNext          V\n");
	}
	else
	{
		printf("BinaryTreeNext          X\n");
	}
	
	iter = BinaryTreePrev(iter);
	iter = BinaryTreePrev(iter);
	if (9 == *(int *)BinaryTreeGetData(iter))
	{
		printf("BinaryTreeNext          V\n");
	}
	else
	{
		printf("BinaryTreeNext          X\n");
	}
	if(30 == *(int *)BinaryTreeGetData(BinaryTreePrev(BinaryTreeEnd(binary_tree))))
	{
		printf("BinaryTreeEnd           V\n");
		printf("BinaryTreePrev          V\n");
	}
	else
	{
		printf("BinaryTreeEnd           X\n");
		printf("BinaryTreePrev          X\n");
	}
	if (1 == is_empty_flag && !BinaryTreeIsEmpty(binary_tree))
	{
		printf("BinaryTreeIsEmpty       V\n");
	}
	else
	{
		printf("BinaryTreeIsEmpty       X\n");
	}
	
	if (NULL != BinaryTreeFind(binary_tree, (void *)&num))
	{
		printf("BinaryTreeFind          V\n");
	}
	else
	{
		printf("BinaryTreeFind          X\n");
	}
	if (14 == BinaryTreeSize(binary_tree))
	{
		printf("BinaryTreeSize          V\n");
	}
	else
	{
		printf("BinaryTreeSize          X\n");
	}
	if (BinaryTreeIsIterEqual(iter, iter))
	{
		printf("BinaryTreeIsIterEqual   V\n");
	}
	else
	{
		printf("BinaryTreeIsIterEqual   X\n");
	}

	iter = BinaryTreeBegin(binary_tree);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	BinaryTreeRemove(iter); 
	printf("after remove 17\n\n\n");
	iter = BinaryTreeBegin(binary_tree);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	iter = BinaryTreeNext(iter);
	printf("\n\nafter destroy\n\n\n");
	BinaryTreeDestroy(binary_tree);
	
	
	/*
	if (16 == *(int *)BinaryTreeGetData(BinaryTreeNext(iter_prev)))
	{
		printf("BinaryTreeRemove         V\n");
	}
	else
	{
		printf("BinaryTreeRemove        X\n");
	}
	*/
	return 0;
	
}

int Copmare(const void *data1, const void *data2)
{
	return *(int *)data1 - *(int *)data2;
}


