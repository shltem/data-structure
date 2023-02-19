
static void TestAllFuncs()
{
    int num1 = 1;
    int num2 = 2;
    int num3 = 3;
    int num4 = 4;
    int num5 = 5;
    int num6 = 6;
    int num7 = 7;
    
    int flag_1 = -1;
    int flag_2 = -1;
    
    node_t node1;
    node_t node2;
    node_t node3;
    node_t node4;
    node_t node5;
    node_t node6;
    node_t node7;
    
    node_ptr_t intersected_1;
    node_ptr_t intersected_2;
    node_ptr_t ptr1 = &node1;
    node_ptr_t ptr5 = &node5;
    
    node1.data = (void*)&num1;
    node2.data = (void*)&num2;
    node3.data = (void*)&num3;
    node4.data = (void*)&num4;
    node5.data = (void*)&num5;
    node6.data = (void*)&num6;
    node7.data = (void*)&num7;
    
    node1.next = &node2;
    node2.next = &node3;
    node3.next = &node4;
    node4.next = NULL;
    node5.next = &node6;
    node6.next = &node7;
    node7.next = NULL;
    
    
    ptr1 = Flip(ptr1);
    
    if((*(int*)ptr1->data == *(int*)node4.data) && 
    (*(int*)ptr1->next->data == *(int*)node3.data))
    {
        printf("Flip Working!                                        V\n");
    }
    else
    {
        printf("Flip NOT Working!                                    X\n");
    }
    
    ptr1 = Flip(ptr1);
    flag_1 = HasLoop(ptr1);
    node4.next = &node2; 
    flag_2 = HasLoop(ptr1);
    
    if((0 == flag_1) && (1 == flag_2))
    {
        printf("HasLoop Working!                                     V\n");
    }
    else
    {
        printf("HasLoop NOT Working!                                  X\n");
    }
    
    node4.next = NULL;
    intersected_1 = FindIntersection(ptr1, ptr5);
    node7.next = &node3;
    intersected_2 = FindIntersection(ptr1, ptr5);
    
    if((NULL == intersected_1) && (intersected_2 == &node3))
    {
        printf("FindIntersection Working!                            V\n");
    }
    else
    {
        printf("FindIntersection NOT Working!                        X\n");
    }
}
