#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "task.h"


void Test();
int AddOne(void *param);
int Minustwo(void *param);

int main()
{
	Test();
	return 0;
}

int AddOne(void *param)
{
	++*(int*)param;
	return (0);
}


int Minustwo(void *param)
{
	--*(int*)param;
	--*(int*)param;
	
	return (0);
}

void Test()
{
	task_t *task_add = NULL;
	task_t *task_minus = NULL;
	UID_t uid_add;
	UID_t uid_minus;
	
	int param = 10;
	long first_run_time = 0;
	int interval_minus = 20;
	int interval_add = 200;
	

	task_add = TaskCreate(AddOne, interval_add, &param);
	task_minus = TaskCreate(Minustwo, interval_minus, &param);
	
	uid_add = TaskGetUID(task_add);
	uid_minus = TaskGetUID(task_minus);
	
	first_run_time = time(NULL);
	TaskRun(task_add);
	TaskRun(task_minus);
	assert(9 == param);
	
	assert(0 == TaskCompare(task_add, uid_minus));
	assert(1 == TaskCompare(task_add, uid_add));
	
	
	assert((first_run_time + interval_add) == TaskGetNextRunTime(task_add));
	assert((first_run_time + interval_minus) == TaskGetNextRunTime(task_minus));


	TaskDestroy(task_add);
	TaskDestroy(task_minus);
}
	
	
	
