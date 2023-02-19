#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hasht.h"
char *strdup(const char *s);
/*#include "utilities.h"cmp func*/
int PrintNode(void *data, void *param);
int Free(void *data, void *param);

int IsMach(const void *key1, const void *key2);
size_t HashFunc(const void *key);


int main()
{
	char buffer[100] = {0};
	FILE *file = NULL;
	char *word_to_insert = NULL;
	size_t capacity = 15000;
	hasht_t *table = NULL;
	char *word_to_search = "Jewish";
	void *data = NULL;


	table = HashtCreate(capacity, IsMach, HashFunc);
	if(NULL == table)
	{
		return -1;
	}
		
	/*for(index = 0; index < 10; ++index)
	{
		num_array[index] = 32 *index;
	}
	
	
	for(index = 0; index < 10; ++index)
	{
		HashtInsert(table, (void*)&num_array[index]);
	}*/

	file = fopen("/usr/share/dict/american-english", "r");
	if (NULL == file)
	{
		return -1;
	}

	while (!feof(file))
	{
		fscanf(file, "%s", buffer);
		word_to_insert = strdup(buffer);
		HashtInsert(table, (void *)word_to_insert);
	}
	fclose(file);

	printf("size %lu\n", HashtSize(table));

	data = HashtFind(table, (void *)word_to_search);
	if(NULL == data)
	{
		printf("word is not in dictionary\n");
	}
	else
	{
		printf("the word **%s** is a word in the dictionary\n", (char *)data);
	}

		printf("is empty: %d\n", HashtIsEmpty(table));
	
	/*HashtRemove(table , &num_to_find);*/
	HashtForEach(table, Free, NULL);
	HashtDestroy(table);
	return 0;
}


size_t HashFunc(const void *key)
{

	char *str = (char *)key;
	unsigned long hash = 5381;
    int charector = 0;

    while (charector == (*str))
    {    
		hash = hash * 33  + charector;
	}
    return hash % 15000;
}

int IsMach(const void *key1, const void *key2)
{
	return (0 == (*(int *)key1 - *(int *)key2));
}

int Free(void *data, void *param)
{
	free(data);
	(void)param;
	return 0;
}