#include <stdio.h>/*malloc ,free*/
#include <assert.h>/*assert*/
#include "avl.h"
#include <stdlib.h>/*calloc*/
#include "utilities.h"/*max*/


typedef struct avl_node avl_node_t;

typedef enum dircetion
{
    LEFT_R,
    RIGHT_R
}dircetion_t;

typedef enum opreation
{
    INSERT ,
    REMOVE,
    DUMMY
}opreation_t;

typedef enum children
{
    LEFT = 0,
    RIGHT,
    NUM_CHILDREN
}children_t;

typedef enum num_children
{
	NO_CHILDREN = 0,
	ONE_CHILD,
	TWO_CHILDREN

} num_children_t;

struct avl_node
{
    void *data;
    avl_node_t *children[NUM_CHILDREN];
    size_t height;
};


struct avl
{
    avl_node_t *root;
    cmp_fun_t cmp_fun;
};

int Compar(const void *data1, const void *data2);
static void DestroyTree(avl_node_t *node);
int CountAvlTree(void *data, void *param);
static size_t GetHheight(avl_node_t *node);
static status_t AVLForEachPOST(avl_node_t *Node, action_func_t action_func, void *param);
static status_t AVLForEachIN(avl_node_t *Node, action_func_t action_func, void *param);
static status_t AVLForEachPRE(avl_node_t *Node, action_func_t action_func, void *param);
static avl_node_t *FindInTree(avl_t *tree, children_t *dircetion, avl_node_t *Node, void *key, opreation_t opreation);
static void RemoveNode(avl_t *tree, avl_node_t *node, const void *key);
static avl_node_t *MinInRight(avl_node_t *node);
static void RemoveRoot(avl_t *tree);
static int GetHeightDiff(avl_node_t *node_to_remove);
static void BalanceTree(avl_node_t *node);
static void RotateTree(avl_node_t *node, dircetion_t dircetion);
static status_t CreateNewNode(avl_node_t **node);

avl_t *AVLCreate(cmp_fun_t cmp_fun)
{
	avl_t *tree = NULL;
	assert(NULL != cmp_fun);

	tree = (avl_t *)calloc(1, sizeof(avl_t));
	if (NULL == tree)
	{
		return NULL;
	}

	tree->cmp_fun = cmp_fun;
	return tree;
}

void AVLDestroy(avl_t *tree)
{
	assert(NULL != tree);
	DestroyTree(tree->root);
	free(tree);
}

static void DestroyTree(avl_node_t *node)
{
	if(NULL == node)
	{
		return;
	}
	
	DestroyTree(node->children[RIGHT]);
	DestroyTree(node->children[LEFT]);
	free(node);
}

void AVLRemove(avl_t *tree, const void *key)
{
   	children_t dircetion = LEFT;
   	avl_node_t *node = NULL;
	if(NULL == tree->root)
    {
		return;
    }
 	if(0 == tree->cmp_fun(key, tree->root->data))
 	{
 		RemoveRoot(tree);
 		return;
 	}
    node = FindInTree(tree, &dircetion, tree->root, (void *)key, REMOVE);
	RemoveNode(tree, node ,key);
}

static void RemoveNode(avl_t *tree, avl_node_t *node, const void *key)
{
	avl_node_t *parent = NULL;
	avl_node_t *temp_node = NULL;
	avl_node_t *node_to_remove = NULL;
    children_t status = NO_CHILDREN;
    children_t dircetion = LEFT;
	children_t child_dircetion = LEFT;

    parent = FindInTree(tree, &dircetion, node, (void *)key, REMOVE);
    
    node_to_remove = parent->children[dircetion];
    
	if (NULL == node_to_remove->children[LEFT] && NULL == node_to_remove->children[RIGHT])
	{
		status = NO_CHILDREN;
	}
	else if (NULL == node_to_remove->children[LEFT] || NULL == node_to_remove->children[RIGHT])
	{
		status = ONE_CHILD;
    	child_dircetion = (NULL != node_to_remove->children[LEFT] ? LEFT : RIGHT);
	}
	else
	{
		status = TWO_CHILDREN;
	}
	switch (status)
	{
	case NO_CHILDREN:
				free(node_to_remove);
			 	parent->children[dircetion] = NULL;
				AVLFind(tree, key);
			 	BalanceTree(parent);
 		return;

	case ONE_CHILD:
			parent->children[dircetion] = node_to_remove->children[child_dircetion];
			free(node_to_remove);
			parent->children[dircetion] = NULL;
			AVLFind(tree, key);
		 	BalanceTree(parent);
			return;

	case TWO_CHILDREN:
	
		temp_node = MinInRight(node_to_remove->children[RIGHT]);
		node_to_remove->data = temp_node->data;
		RemoveNode(tree, node_to_remove, temp_node->data);
		return;
	}

}

static void RemoveRoot(avl_t *tree)
{
	avl_node_t *temp_node = NULL;
	avl_node_t *node_to_remove = NULL;
    children_t status = NO_CHILDREN;
   	children_t child_dircetion = LEFT;
   	
	assert(NULL != tree);
	node_to_remove = tree->root;
	
	if (NULL == node_to_remove->children[LEFT] && NULL == node_to_remove->children[RIGHT])
	{
		status = NO_CHILDREN;
	}
	else if (NULL == node_to_remove->children[LEFT] || NULL == node_to_remove->children[RIGHT])
	{
		status = ONE_CHILD;
    	child_dircetion = (NULL != node_to_remove->children[LEFT] ? LEFT : RIGHT);
	}
	else
	{
		status = TWO_CHILDREN;
	}
	switch (status)
	{
	case NO_CHILDREN:
				free(node_to_remove);
			 	tree->root = NULL;
		return;

	case ONE_CHILD:

			temp_node = node_to_remove->children[child_dircetion]->data;
			node_to_remove->data = node_to_remove->children[child_dircetion]->data;
			node_to_remove->children[RIGHT] = node_to_remove->children[child_dircetion]->children[RIGHT];
			node_to_remove->children[LEFT] = node_to_remove->children[child_dircetion]->children[LEFT];			
			free(temp_node);
		 	BalanceTree(node_to_remove);
			
			return;

	case TWO_CHILDREN:
	
		temp_node = MinInRight(node_to_remove->children[RIGHT]);
		node_to_remove->data = temp_node->data;
		RemoveNode(tree, node_to_remove, temp_node->data);
		tree->root->height = GetHheight(tree->root);
		return;
	}
}


static avl_node_t *MinInRight(avl_node_t *node)
{

	if(NULL == node->children[LEFT])
	{
		return node;
	}
	
	return MinInRight(node->children[LEFT]);
}

int AVLInsert(avl_t *tree, void *data)
{
	status_t status = SUCSESS;
    children_t dircetion = LEFT;
    avl_node_t *temp_node = NULL ;
    if(NULL == tree->root)
    {
 	    	CreateNewNode(&tree->root);
 	    	if(SUCSESS != status)
 	    	{
 	    		return -1;
 	    	}
 	    	tree->root->data = data;
 	    	tree->root->height = GetHheight(tree->root);
    }
    else
    {
		temp_node = FindInTree(tree, &dircetion, tree->root, data, INSERT);
		CreateNewNode(&(temp_node->children[dircetion]));
		if(SUCSESS != status)
 	    {
 	    		return -1;
 	   	}
		
		temp_node->children[dircetion]->data = data;
		temp_node->children[dircetion]->height = 1;
		tree->root->height = GetHheight(tree->root);
		AVLFind(tree, data);
		BalanceTree(temp_node);
		AVLFind(tree, data);
	}
	
	return 0;
}

static status_t CreateNewNode(avl_node_t **new_node)
{
	
	*new_node = (avl_node_t *)calloc(1, sizeof(avl_node_t));
	if(NULL == new_node)
	{
		return ERROR;
	}
	
	return SUCSESS;
}

void *AVLFind(const avl_t *tree, const void *key)
{
    avl_node_t *node = NULL;
    children_t dircetion = LEFT;
    node = FindInTree((avl_t *)tree, &dircetion, tree->root, (void *)key, DUMMY);
    return (void *)node->children[dircetion];
}

static avl_node_t *FindInTree(avl_t *tree, children_t *dircetion, avl_node_t *node, void *key, opreation_t opreation)
{
    *dircetion = ((0 > tree->cmp_fun(key, node->data)) ? LEFT : RIGHT);
    if (NULL ==  node->children[*dircetion] || 0 == tree->cmp_fun(key, node->children[*dircetion]->data))
    {
    	if (INSERT == opreation)
    	{
    		node->height = 1 + MAX(1, GetHheight(node->children[!*dircetion]));
    		BalanceTree(node);
    	}
    	
        return node;        
    }
  
    *dircetion = ((0 > tree->cmp_fun(key, node->data)) ? LEFT : RIGHT);
 	node->height = GetHheight(node);
	BalanceTree(node);
    return FindInTree(tree, dircetion, node->children[*dircetion], key, opreation);
}

static int GetHeightDiff(avl_node_t *node)
{
	return GetHheight(node->children[LEFT]) - GetHheight(node->children[RIGHT]);
}

static void BalanceTree(avl_node_t *node)
{
	
	if (1 < GetHeightDiff(node))
	{
		if(0 > GetHeightDiff(node->children[LEFT]))
		{
			RotateTree(node->children[LEFT], LEFT_R);
		}
		
		RotateTree(node, RIGHT_R);
	}
	if (-1 > GetHeightDiff(node))
	{
		if(0 < GetHeightDiff(node->children[RIGHT]))
		{	
			RotateTree(node->children[RIGHT], RIGHT_R);
		}		
		RotateTree(node, LEFT_R);
	}

}


static void RotateTree(avl_node_t *node, dircetion_t dircetion)
{
	avl_node_t *first_tree = NULL;
	avl_node_t *second_tree = NULL;
	avl_node_t *therd_tree = NULL;
	void *temp_data = NULL;
	assert(NULL != node);

	first_tree = node->children[dircetion];
	second_tree = node->children[!dircetion]->children[dircetion];
	therd_tree = node->children[!dircetion]->children[!dircetion];
	
	temp_data = node->data;
	node->data = node->children[!dircetion]->data;
	node->children[!dircetion]->data = temp_data;
	
	node->children[dircetion] = node->children[!dircetion];
	node->children[dircetion]->children[dircetion] = first_tree;
	node->children[dircetion]->children[!dircetion] = second_tree;
	node->children[!dircetion] = therd_tree;
	
	node->height = GetHheight(node);
	node->children[dircetion]->height = GetHheight(node->children[dircetion]);
}

int AVLIsEmpty(const avl_t *tree)
{
	assert(NULL != tree);
	return (NULL == tree->root);
}

size_t AVLHeight(const avl_t *tree)
{	
	assert(NULL != tree);
    return tree->root->height;
}


int AVLSize(const avl_t *tree)
{
	size_t counter = 0;
	AVLForEach((avl_t *)tree, CountAvlTree, &counter, IN_ORDER);
	return counter;
}

static size_t GetHheight(avl_node_t *node)
{
	if(NULL == node)
	{
		return 0;
	}

	node->height = 1 + MAX(GetHheight(node->children[RIGHT]), GetHheight(node->children[LEFT]));
	return node->height;
}

int AVLForEach(avl_t *tree, action_func_t action_func, void *param, traverse_t type)
{
    if (NULL == tree->root)
    {
    	return 0;
    }
    
    switch (type)
    {
        case PRE_ORDER:
            return AVLForEachPRE(tree->root, action_func, param);
        case IN_ORDER:
            return AVLForEachIN(tree->root, action_func, param);
        case POST_ORDER:
            return AVLForEachPOST(tree->root, action_func, param);
    }

    return 0;
}

static status_t AVLForEachPRE(avl_node_t *node, action_func_t action_func, void *param)
{
   	status_t status = SUCSESS;
    if (NULL == node) 
    {
    	return SUCSESS ;
    }	
    status = action_func(node->data, param);
    if(SUCSESS != status)
    {
        return ERROR;
    }
    status = AVLForEachPRE(node->children[LEFT],action_func, param);
    if(SUCSESS != status)
    {
        return ERROR;
    }
    status = AVLForEachPRE(node->children[RIGHT], action_func, param);
        if(SUCSESS != status)
    {
        return ERROR;
    }
    return SUCSESS;
}

static status_t AVLForEachIN(avl_node_t *node, action_func_t action_func, void *param)
{
    status_t status = SUCSESS;
    if(NULL == node)
    {	
    	return SUCSESS;
    }	
    status = AVLForEachIN(node->children[LEFT], action_func, param);
    if(SUCSESS != status)
    {
        return ERROR;
    }
    status = action_func(node->data, param);
    if(SUCSESS != status)
    {
        return ERROR;
    }
    status = AVLForEachIN(node->children[RIGHT], action_func, param);
    if(SUCSESS != status)
    {
        return ERROR;
    }

    return SUCSESS;
}
        
static status_t AVLForEachPOST(avl_node_t *node, action_func_t action_func, void *param)
{
    status_t status = SUCSESS;

    if (NULL == node) 
    {
    	return SUCSESS;
    }	
    status = AVLForEachPOST(node->children[LEFT],action_func, param);
    if(SUCSESS != status)
    {
        return ERROR;
    }
    status = AVLForEachPOST(node->children[RIGHT],action_func, param);
    if(SUCSESS != status)
    {
        return ERROR;
    }
    status = action_func(node->data, param);
    if(SUCSESS != status)
    {
        return ERROR;
    }
    return SUCSESS;
}   


void *AVLGetData(const avl_node_t *node)
{
	return node->data;
}

int CountAvlTree(void *data, void *param)
{
	size_t *counter = (size_t *)param;
	(void)data;
	*counter += 1;

	return 0;
}

int Compar(const void *data1, const void *data2)
{
    return *(int *)data1 - *(int *)data2;
}

int PrintNode(void *data, void *param)
{
	printf("PrintNode: ");
		printf("data = %d ", *(int *)data);
	printf("\n");
	(void)param;
	return 0;
}
