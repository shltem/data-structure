#include <pthread.h>/*ptread create pthread join*/
#include <stdio.h>/*printf*/
#include "UID.h"

#define SIZE 100
#define NUM_OF_UIDS 100
#define NUM_OF_THREADS 10
UID_t uids[NUM_OF_UIDS];

void *CreateUID1(void *index)
{
	int i = (size_t)index * NUM + 1;
	int limit = i + NUM;
	for (; i < limit; i++)
	{
		uids[i] = UIDCreate();
	}
	return NULL;
}

int main()
{
	pthread_t threads[NUM_OF_THREADS];
	size_t i = 0, j = 0;
    int flag = 0;
	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_create(&threads[i], NULL, CreateUID1, (void *)i);
	}

	for (i = 0; i < NUM_OF_THREADS; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	for (i = 0; i < NUM_OF_UIDS; i++)
	{
		for (j = i + 1; j < NUM_OF_UIDS; j++)
		{
			if(UIDIsSame(uids[i], uids[j]) && i != j)
			{
                flag = 1;
            }
		}
	}
    if(0 == flag)
    {
        printf("UID is thread safety\n");
    }

	return 0;
}
