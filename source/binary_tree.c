#include <assert.h>/*assert*/
#include <stdlib.h>/*malloc free*/
#include "stack.h"
#include "binary_tree.h"

#define UNUSED(x) (void)(x)

struct binary_tree;

struct binary_tree_node;

typedef struct binary_tree_node tree_node_t;

typedef enum dircetion
{
	NEXT = 0,
	PREV

} dircetion_t;

typedef enum num_children
{
	NO_CHILDREN = 0,
	ONE_CHILD,
	TWO_CHILDREN

} num_children_t;

typedef enum children
{
	LEFT = 0,
	RIGHT,
	NUM_CHILDREN

} children_t;

struct binary_tree_node
{
	void *data;
	tree_node_t *parent;
	tree_node_t *children[NUM_CHILDREN];
};

struct binary_tree
{
	tree_node_t end_stub;
	compare_t compare_func;
};

static tree_node_t *BinaryTreeIterative(tree_node_t *node, dircetion_t dircetion);
static tree_node_t *CreateNewNode(tree_node_t *node_to_insert, children_t dircetion, void *data);
static tree_node_t *RemoveNode(binary_tree_iterator_t iter);
static tree_node_t *FindPlaceInTree(binary_tree_t *binary_tree, children_t *dircetion, void *data);
static void PrintNode(tree_node_t *nodes);
static binary_tree_iterator_t NodeToIter(tree_node_t *node);
static tree_node_t *IterToNode(binary_tree_iterator_t iter);
int CountTree(void *data, void *param);

binary_tree_t *BinaryTreeCreate(compare_t compare_func)
{
	binary_tree_t *binary_tree = NULL;
	assert(NULL != compare_func);

	binary_tree = (binary_tree_t *)calloc(1, sizeof(binary_tree_t));
	if (NULL == binary_tree)
	{
		return NULL;
	}

	binary_tree->compare_func = compare_func;

	return binary_tree;
}

void BinaryTreeDestroy(binary_tree_t *binary_tree)
{
	tree_node_t *runner = NULL;
	tree_node_t *node_to_remove = NULL;
	assert(NULL != binary_tree);

	runner = IterToNode(BinaryTreeBegin(binary_tree));
	while (&binary_tree->end_stub != runner)
	{
		node_to_remove = runner;
		runner = IterToNode(BinaryTreeNext(NodeToIter(runner)));
		BinaryTreeRemove(NodeToIter(node_to_remove));
	}

	free(binary_tree);
}

void BinaryTreeRemove(binary_tree_iterator_t iter)
{
	assert(NULL != iter);
	RemoveNode(NodeToIter(iter));
}

binary_tree_iterator_t BinaryTreeNext(binary_tree_iterator_t iter)
{
	tree_node_t *runner = NULL;
	dircetion_t dircetion = NEXT;

	assert(NULL != iter);
	runner = IterToNode(iter);
	PrintNode(runner);

	runner = BinaryTreeIterative(runner, dircetion);
	return NodeToIter(runner);
}

binary_tree_iterator_t BinaryTreePrev(binary_tree_iterator_t iter)
{
	tree_node_t *runner = NULL;
	dircetion_t dircetion = PREV;

	assert(NULL != iter);

	runner = IterToNode(iter);
	runner = BinaryTreeIterative(runner, dircetion);

	return NodeToIter(runner);
}

static tree_node_t *BinaryTreeIterative(tree_node_t *runner, dircetion_t dircetion)
{

	children_t child_dircetion = LEFT;
	children_t child_other_dircetion = LEFT;
	dircetion_t oter_dirction = LEFT;

	child_dircetion = (dircetion == NEXT ? RIGHT : LEFT);
	child_other_dircetion = (child_dircetion == RIGHT ? LEFT : RIGHT);
	assert(NULL != runner);

	if (NULL != runner->children[child_dircetion])
	{
		runner = runner->children[child_dircetion];

		while (NULL != runner->children[child_other_dircetion])
		{
			runner = runner->children[child_other_dircetion];
		}
		return runner;
	}

	else
	{
		while (runner->parent->children[child_dircetion] == runner)
		{
			runner = runner->parent;
		}
		return runner->parent;
	}

	return runner;
}

binary_tree_iterator_t BinaryTreeInsert(binary_tree_t *binary_tree, void *data)
{
	tree_node_t *node_to_insert = NULL;
	children_t dircetion = LEFT;
	assert(NULL != binary_tree);

	node_to_insert = FindPlaceInTree(binary_tree, &dircetion, data);
	node_to_insert = CreateNewNode(node_to_insert, dircetion, data);
	if (NULL == node_to_insert)
	{
		return BinaryTreeEnd(binary_tree);
	}

	return NodeToIter(node_to_insert->children[dircetion]);
}

static tree_node_t *CreateNewNode(tree_node_t *node_to_insert, children_t dircetion, void *data)
{
	assert(NULL != node_to_insert);
	node_to_insert->children[dircetion] = (tree_node_t *)calloc(1, sizeof(tree_node_t));
	if (NULL == node_to_insert->children[dircetion])
	{
		return NULL;
	}

	node_to_insert->children[dircetion]->data = data;
	node_to_insert->children[dircetion]->parent = node_to_insert;

	return node_to_insert;
}

binary_tree_iterator_t BinaryTreeBegin(const binary_tree_t *binary_tree)
{
	tree_node_t *runner = NULL;

	assert(NULL != binary_tree);
	runner = (tree_node_t *)&binary_tree->end_stub;

	while (NULL != runner->children[LEFT])
	{
		runner = runner->children[LEFT];
	}

	return NodeToIter(runner);
}

binary_tree_iterator_t BinaryTreeEnd(const binary_tree_t *binary_tree)
{
	assert(NULL != binary_tree);

	return NodeToIter((tree_node_t *)&binary_tree->end_stub);
}

int BinaryTreeIsEmpty(const binary_tree_t *binary_tree)
{
	assert(NULL != binary_tree);

	return (NULL == binary_tree->end_stub.children[LEFT]);
}

void *BinaryTreeGetData(const binary_tree_iterator_t iter)
{
	tree_node_t *node = NULL;

	assert(NULL != iter);

	node = IterToNode(iter);

	return node->data;
}

binary_tree_iterator_t BinaryTreeFind(const binary_tree_t *binary_tree, void *key_to_data)
{
	tree_node_t *runner = NULL;
	children_t dircetion = LEFT;
	assert(NULL != binary_tree);

	runner = FindPlaceInTree((binary_tree_t *)binary_tree, &dircetion, key_to_data);

	if (0 == binary_tree->compare_func(key_to_data, runner->data))
	{
		return NodeToIter(runner);
	}
	else
	{
		return NULL;
	}
}

int BinaryTreeForEach(binary_tree_iterator_t from,
					  binary_tree_iterator_t to,
					  action_t action_func,
					  void *param)
{
	int status = 0;
	tree_node_t *runner = NULL;

	assert(NULL != from);
	assert(NULL != to);

	runner = IterToNode(from);

	while (!BinaryTreeIsIterEqual(runner, IterToNode(to)))
	{
		status = action_func(runner->data, param);
		if (0 != status)
		{
			break;
		}
		runner = BinaryTreeNext(runner);
	}
	return status;
}

size_t BinaryTreeSize(const binary_tree_t *binary_tree)
{
	size_t counter = 0;

	BinaryTreeForEach(BinaryTreeBegin(binary_tree),
					  NodeToIter((tree_node_t *)&binary_tree->end_stub),
					  CountTree, (void *)&counter);

	return counter;
}

static binary_tree_iterator_t NodeToIter(tree_node_t *node)
{
	assert(NULL != node);

	return (binary_tree_iterator_t)node;
}

static tree_node_t *IterToNode(binary_tree_iterator_t iter)
{
	assert(NULL != iter);

	return (tree_node_t *)iter;
}

int BinaryTreeIsIterEqual(const binary_tree_iterator_t iter_one, const binary_tree_iterator_t iter_two)
{
	assert(NULL != iter_one);
	assert(NULL != iter_two);

	return (iter_one == iter_two);
}

static tree_node_t *FindPlaceInTree(binary_tree_t *binary_tree,
									children_t *dircetion,
									void *key_to_data)
{
	tree_node_t *runner = NULL;
	assert(NULL != binary_tree);
	assert(NULL != dircetion);
	assert(NULL != key_to_data);

	if (!BinaryTreeIsEmpty(binary_tree))
	{
		runner = binary_tree->end_stub.children[LEFT];
	}
	else
	{
		return &binary_tree->end_stub;
	}

	*dircetion = ((0 > binary_tree->compare_func(key_to_data, runner->data)) ? LEFT : RIGHT);

	while (0 != binary_tree->compare_func(key_to_data, runner->data))
	{
		if (NULL == runner->children[*dircetion])
		{
			break;
		}
		runner = runner->children[*dircetion];
		*dircetion = ((0 > binary_tree->compare_func(key_to_data, runner->data)) ? LEFT : RIGHT);
	}

	return runner;
}

static void PrintNode(tree_node_t *node)
{
	printf("\n PrintNode: ");

	if (NULL != node->data)
	{
		printf("data = %02d ", *(int *)node->data);
	}
	if (NULL != node->parent && NULL != node->parent->parent)
	{
		printf("parent = %02d ", *(int *)node->parent->data);
	}
	else
		printf("            ");
	if (NULL != node->children[LEFT])
	{
		printf("LEFT = %02d ", *(int *)node->children[LEFT]->data);
	}
	else
		printf("          ");
	if (NULL != node->children[RIGHT])
	{
		printf("RIGHT = %02d ", *(int *)node->children[RIGHT]->data);
	}
	else
		printf("            ");
}

int CountTree(void *data, void *param)
{
	size_t *counter = (size_t *)param;
	UNUSED(data);
	*counter += 1;

	return 0;
}

static tree_node_t *RemoveNode(binary_tree_iterator_t iter)
{
	tree_node_t *node_to_remove = NULL;
	tree_node_t *prev_node_to_remove = NULL;
	tree_node_t *next_node = NULL;
	num_children_t status = NO_CHILDREN;

	children_t parent_dircetion = LEFT;
	children_t child_dircetion = LEFT;

	assert(NULL != iter);
	node_to_remove = IterToNode(iter);

	next_node = IterToNode(BinaryTreeNext(NodeToIter(node_to_remove)));

	if (NULL == node_to_remove->children[LEFT] && NULL == node_to_remove->children[RIGHT])
	{
		status = NO_CHILDREN;
	}
	else if (NULL == node_to_remove->children[LEFT] || NULL == node_to_remove->children[RIGHT])
	{
		status = ONE_CHILD;
		parent_dircetion = (node_to_remove->parent->children[LEFT] == node_to_remove ? LEFT : RIGHT);
		child_dircetion = (NULL != node_to_remove->children[LEFT] ? LEFT : RIGHT);
	}
	else
	{
		status = TWO_CHILDREN;
	}

	switch (status)
	{
	case NO_CHILDREN:
		node_to_remove->parent->children[parent_dircetion] = NULL;
		break;

	case ONE_CHILD:
		node_to_remove->parent->children[parent_dircetion] =
			node_to_remove->children[child_dircetion];
		node_to_remove->children[child_dircetion]->parent = node_to_remove->parent;
		break;

	case TWO_CHILDREN:

		prev_node_to_remove = BinaryTreePrev(NodeToIter(node_to_remove));
		node_to_remove->data = prev_node_to_remove->data;
		RemoveNode(prev_node_to_remove);
		return NodeToIter(next_node);
	}

	node_to_remove->data = NULL;
	node_to_remove->parent = NULL;
	node_to_remove->children[LEFT] = NULL;
	node_to_remove->children[RIGHT] = NULL;

	free(node_to_remove);

	return NodeToIter(next_node);
}
