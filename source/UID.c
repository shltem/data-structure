#include <pthread.h>/*mutex*/

#include "UID.h"
static float general_counter;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

const UID_t badUID = {0, 0, 0};

UID_t UIDCreate(void)
{
	UID_t uid = badUID;
	pthread_mutex_lock(&lock);
	uid.clock = clock();
	uid.pid = getpid();
	++general_counter;
	uid.counter =  general_counter;
	pthread_mutex_unlock(&lock);
	
	return uid;
}

int UIDIsSame(UID_t UID1, UID_t UID2)
{
	return ((0 == difftime(UID1.clock, UID2.clock) && (UID1.pid == UID2.pid) && (UID1.counter == UID2.counter)));
}
