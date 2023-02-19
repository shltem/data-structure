#include <stdlib.h>/*malloc clloc free atoi*/
#include <stdio.h>/*sprintf*/
#include <assert.h>/*assert*/
#include "dhcp.h"
#define PERIOD_LENTH (1)
#define NUM_OF_BITS_IN_BYTE (8)
#define BYTE_MASK (255)
#define NUM_BITS_IN_IP 32
#define TRUE (1)
#define FALSE (0)
#define SUCCESS (0)
#define FAIL (1)

/*====================== Enum ==============================================*/

typedef enum children
{
	LEFT = 0,
	RIGHT,
	NUM_OF_CHILDREN
}children_t;

/*============================================================================*/


/*========================== structs =========================================*/


typedef struct dhcp_node dhcp_node_t;

struct dhcp
{
	dhcp_node_t *root;
	uint32_t subnet_ID;
	size_t mask_length;
};


struct dhcp_node
{
	dhcp_node_t *parent;
	dhcp_node_t *children[NUM_OF_CHILDREN];
	int is_full;
};

/*============================================================================*/

/*===============================function decleration ========================*/

static size_t NumOfDigits(int num);
static dhcp_node_t *CreateNewNode(dhcp_node_t *node, children_t direction);
static void DestroyNodsRec(dhcp_node_t *node);
static void UpdateFullness(dhcp_node_t *node);
static size_t DHCPCountFreeRec(dhcp_node_t *node, int height);
static dhcp_node_t *FreeNodsRec(dhcp_node_t *node);
void PrintDhcp(dhcp_t *dhcp);
static void PrintNodeRec(dhcp_node_t *node, uint32_t subnet_ID, uint32_t offset);

/*============================================================================*/


/*========================= dhcp functions =====================================*/


dhcp_t *DHCPCreate(uint32_t subnet_ID, size_t subnet_length)
{
	uint32_t result_ip = 0 ;
	uint32_t requested_ip = 0 ;

	dhcp_t *dhcp = (dhcp_t *)calloc(1, sizeof(dhcp_t));
	if(NULL == dhcp)
	{
		return NULL;
	}
	
	dhcp->root = (dhcp_node_t *)calloc(1, sizeof(dhcp_node_t));
	if(NULL == dhcp->root)
	{
		free(dhcp);
		return NULL;
	}
	
	dhcp->subnet_ID = subnet_ID & ((~0)<<(NUM_BITS_IN_IP-subnet_length)); /*promןs set off bits after subnet lenth*/
	dhcp->mask_length = subnet_length;

	requested_ip = dhcp->subnet_ID ; /*network address X.000...000 */
	DHCPAllocateIp(dhcp, requested_ip, &result_ip);
	requested_ip = dhcp->subnet_ID + (1<<(NUM_BITS_IN_IP-subnet_length))-2; /*server address X.111...110 (-2: first -1 to set all bits after subnet to ones and after that anther -1 to set last bit to zero*/
	DHCPAllocateIp(dhcp, requested_ip, &result_ip);
	requested_ip = dhcp->subnet_ID + (1<<(NUM_BITS_IN_IP-subnet_length))-1; /*broadcast address X.111...111 (-1 to set all bits after subnet to ones*/
	DHCPAllocateIp(dhcp, requested_ip, &result_ip);

	return dhcp;
}

void DHCPDDestroy(dhcp_t *dhcp)
{
	DestroyNodsRec(dhcp->root);
	free(dhcp);
}

static void DestroyNodsRec(dhcp_node_t *node)
{
	if(NULL == node)
	{
		return;
	}
	
	DestroyNodsRec(node->children[LEFT]);
	DestroyNodsRec(node->children[RIGHT]);
	free(node);
}

static dhcp_node_t *FreeNodsRec(dhcp_node_t *node)
{
	dhcp_node_t *parent = NULL ;
	children_t direction = LEFT;
	if (NULL == node->parent)
	{
		return NULL;
	}
	
	if ((NULL != node->children[LEFT]) || (NULL != node->children[RIGHT]))
	{
		return node;
	}
	
	direction = ((node == node->parent->children[LEFT]) ? LEFT : RIGHT);
	parent = node->parent;
	free(node);
	parent->children[direction] = NULL ;
	return FreeNodsRec(parent);
}

void DHCPFreeIp(dhcp_t *dhcp, uint32_t ip)
{
    int height = 0;
	dhcp_node_t *node_for_fullness = NULL;
	dhcp_node_t *node = NULL;
	children_t direction = LEFT;
	assert(NULL != dhcp);
    
	node = dhcp->root;
	for(height = NUM_BITS_IN_IP - dhcp->mask_length-1; height >= 0; --height)
	{
		direction = (((ip >> height) & 1) ? RIGHT : LEFT);
		if(NULL == node->children[direction])
		{
			return;
		}		
		node = node->children[direction];
	}	
	node_for_fullness = FreeNodsRec(node) ;
	UpdateFullness(node_for_fullness);   
	return;
}

static int IsNotFull(dhcp_node_t *node)
{
	if (NULL == node)
	{
		return 1;
	}
	return !(node->is_full);
}


int DHCPAllocateIp(dhcp_t *dhcp, uint32_t requested_ip, uint32_t *result_ip)
{
    int height = 0;
	dhcp_node_t *curent_node = NULL;
	dhcp_node_t *node = NULL;
	children_t direction = LEFT;

	assert(NULL != dhcp);

	requested_ip = 
	((dhcp->subnet_ID == (requested_ip & ((~0)<<(NUM_BITS_IN_IP-dhcp->mask_length))))
	? requested_ip : dhcp->subnet_ID ); /* chek that subnet is correct in requested ip*/

	node = dhcp->root;
	for(height = NUM_BITS_IN_IP - dhcp->mask_length-1; height >= 0; --height)
	{
		direction = (((requested_ip >> height) & 1) ? RIGHT : LEFT);
		if(NULL == node->children[direction])
		{
			CreateNewNode(node, direction);
			if(NULL == node->children[direction])
			{
				return FAIL;
			}
		}	
		if(node->children[direction]->is_full)
		{
			if(RIGHT == direction)
			{
				do
				{
					if(NULL == node->parent)
					{
						return 1;
					}
					++height;
					curent_node = node;
					node = node->parent;

				} while ((curent_node == node->children[RIGHT]) || (node->children[LEFT])->is_full);
		
			}
			if(NULL == node->children[RIGHT])
			{
				CreateNewNode(node, RIGHT);
				if(NULL == node->children[RIGHT])
				{
					return FAIL;
				}
			}	
			direction = RIGHT;
			requested_ip = (requested_ip | (1 << height)) & (~((1 << height) - 1)); /* bit set on and turn of till the height: -1 make the number all ones and then ~ and set off for compliting the work */
		}
		
		node = node->children[direction];
	}	
	
	if(node->is_full) /* trie is full */
    {
        return FAIL;
    }

	*result_ip = requested_ip;
	UpdateFullness(node);    
    
	return SUCCESS;
}

static dhcp_node_t *CreateNewNode(dhcp_node_t *node, children_t direction)
{
	node->children[direction] = (dhcp_node_t *)calloc(1, sizeof(dhcp_node_t));
	if(NULL != node->children[direction])
	{
		node->children[direction]->parent = node ;
	}
	return node->children[direction];
}

char *DHCPIpToString(uint32_t ip, char *dest_ip)
{
	sprintf(dest_ip, "%d.%d.%d.%d", (ip >> 24) , 
	(ip >> 16) & BYTE_MASK , (ip >> 8) & BYTE_MASK , ip & BYTE_MASK) ;
	return dest_ip;
}

uint32_t DHCPStringToIp(char *ip)
{
	int index = 0;
	uint32_t num_ip_to_return = 0;
	int num = 0;

	for(index = 3; index >= 0; --index)
	{
		num = atoi(ip);
		ip += NumOfDigits(num) + PERIOD_LENTH;
		num_ip_to_return += num << (NUM_OF_BITS_IN_BYTE * index);
	}

	return num_ip_to_return;
}

size_t DHCPCountFree(dhcp_t *dhcp)
{
	return DHCPCountFreeRec(dhcp->root, (NUM_BITS_IN_IP - dhcp->mask_length));
}

static size_t DHCPCountFreeRec(dhcp_node_t *node, int height)
{
	size_t count_free = 0;
	
	if (height < 0)
	{
		return 0;
	}

	if (NULL == node)
	{
		return (1 << height); /* 2 ^ height*/
	}

	count_free += DHCPCountFreeRec(node->children[LEFT], height -1);
	count_free += DHCPCountFreeRec(node->children[RIGHT], height -1);

	return count_free;
}


static size_t NumOfDigits(int num)
{
	size_t num_of_digit = 0;
	
	do
	{
		++num_of_digit;
		num /= 10;
	}while(num);

	return num_of_digit;

}

static void UpdateFullness(dhcp_node_t *node)
{
	if (NULL == node)
	{
		return; 
	}
	if (NULL == node->children[LEFT] && NULL == node->children[RIGHT])
    {
        node->is_full = TRUE;
    }
	else if (IsNotFull(node->children[LEFT]) || IsNotFull(node->children[RIGHT]))
	{
		node->is_full = FALSE ;
	}
	else 
	{
		node->is_full = TRUE ;
	}
	
	UpdateFullness(node->parent);	
}

void PrintDhcp(dhcp_t *dhcp)
{
	PrintNodeRec(dhcp->root, dhcp->subnet_ID, 0);
}

static void PrintNodeRec(dhcp_node_t *node, uint32_t subnet_ID, uint32_t offset)
{
	char buffer[20] = {0};
	if(NULL == node)
	{
		return;
	}
	PrintNodeRec(node->children[LEFT], subnet_ID, offset << 1);
	PrintNodeRec(node->children[RIGHT], subnet_ID, ((offset << 1) + RIGHT));
	if((NULL == node->children[LEFT]) && (NULL ==node->children[RIGHT]))
	{	
		printf("%s\n", DHCPIpToString(subnet_ID+offset, buffer));
	}
}

