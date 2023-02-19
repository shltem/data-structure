#include "avl.h"
#include <stdio.h>/*printf*/

int ComparKey(const void *data1, const void *data2);

int main()
{
    int index = 0;
    void *param = NULL;
    avl_t *tree = NULL;
    int data[] = {6, 2, 7, 1, 4, 9, 3, 5, 8};
    tree = AVLCreate(ComparKey);
    printf("is empty %d\n", AVLIsEmpty(tree));
    for (index=0; index<9; ++index)
    {
        AVLInsert(tree, (void *)&data[index]);
        AVLForEach(tree, PrintNode, param, IN_ORDER);
        printf("\n\n");
    }    
/*    node = (avl_node_t *)AVLFind(tree, (void *)&data[4]);
    printf("*********************AVLFindMain num %d\n", *(int *)AVLGetData(node));
    printf("is empty %d\n", AVLIsEmpty(tree));
    printf("IN_ORDER:\n\n");
    AVLForEach(tree, PrintNode, param, IN_ORDER);
   printf("size %lu\n", AVLSize(tree));	
	printf("PRE_ORDER:\n\n");
    AVLForEach(tree, PrintNode, param, PRE_ORDER);
         printf("POST_ORDER:\n\n");
    AVLForEach(tree, PrintNode, param, POST_ORDER);
*/	

        printf("after remove height %lu\n", AVLHeight(tree));
AVLRemove(tree, (void *)&data[3]);
printf("IN_ORDER:\n\n");
    AVLForEach(tree, PrintNode, param, IN_ORDER);
/*    printf("height %lu\n", AVLHeight(tree));
   printf("size %lu\n", AVLSize(tree));
	
	AVLRemove(tree, (void *)&data[7]);
printf("IN_ORDER:\n\n");
    AVLForEach(tree, PrintNode, param, IN_ORDER);
    printf("height %lu\n", AVLHeight(tree));
   printf("size %lu\n", AVLSize(tree));

AVLRemove(tree, (void *)&data[6]);
printf("IN_ORDER:\n\n");
    AVLForEach(tree, PrintNode, param, IN_ORDER);
    printf("height %lu\n", AVLHeight(tree));
   printf("size %lu\n", AVLSize(tree));
  */
   AVLDestroy(tree);
    return 0;
}

int ComparKey(const void *data1, const void *data2)
{
    return *(int *)data1 - *(int *)data2;
}
