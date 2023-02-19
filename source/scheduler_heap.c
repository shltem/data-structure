#include <stdlib.h>/*malloc free*/
#include <assert.h>/*assert*/

#include "scheduler_heap.h"

static int CompareNextRunTime(const void *data1, const void *data2);
static int CompareTaskUid(const void *data, const void *param);

struct scheduler
{
	bin_heap_t *heap;
	int stop_flag;
};


scheduler_t* SchedulerCreate(void)
{
	scheduler_t *scheduler =(scheduler_t *)malloc(sizeof(scheduler_t));
	if(NULL == scheduler)
	{
		return NULL;
	}
	scheduler->heap = BinHeapCreate(CompareNextRunTime);
	if(NULL == scheduler->heap)
	{
		free(scheduler);
		return NULL;
	}
	scheduler->stop_flag = 0;
	
	return scheduler;
}

void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	
	SchedulerClear(scheduler);
	BinHeapDestroy(scheduler->heap);
	scheduler->heap = NULL;
	
	free(scheduler);
}

UID_t SchedulerAddTask(scheduler_t *scheduler, action_func *func, void* param, size_t interval)
{
	
	task_t *task = NULL; 
	return_type_t status = success;
	
	assert(NULL != scheduler);
	assert(NULL != func);

	
	task = TaskCreate(func, interval, param);
	if (NULL == task)
	{
		return badUID;
	}
	
	status = BinHeapPush(scheduler->heap, (void *)task);
	if (success != status)
	{
		TaskDestroy(task);
		return badUID;
	}
	
	return TaskGetUID(task);
}

int SchedulerRemoveTask(scheduler_t *scheduler, UID_t uid)
{
	task_t *task = NULL;
	return_type_t status = success;
	
	assert(NULL != scheduler);
	
	task = (task_t *)BinHeapRemove(scheduler->heap, CompareTaskUid, &uid);
	if (success != status)
	{
		return fail;
	}
	
	TaskDestroy(task);

	return 0;
}

void SchedulerClear(scheduler_t *scheduler)
{
	task_t *current_task = NULL;
	assert(NULL != scheduler);

	while(!BinHeapIsEmpty(scheduler->heap))
	{
		current_task = BinHeapPop(scheduler->heap);
		TaskDestroy(current_task);
	}
}

int SchedulerRun(scheduler_t *scheduler)
{
	return_type_t status = success;
	int status_cercler = 1;
	double wait_time = 0;
	task_t *current_task = NULL;
	
	assert(NULL != scheduler);
	
	while(!SchedulerIsEmpty(scheduler) && !scheduler->stop_flag)
	{
		current_task = BinHeapPeek(scheduler->heap);
		wait_time = difftime(TaskGetNextRunTime(current_task), time(NULL));
		while(0 < wait_time)
		{
			sleep(wait_time);
			wait_time = difftime(TaskGetNextRunTime(current_task), time(NULL));
		}
		status_cercler = TaskRun(current_task);
		BinHeapPop(scheduler->heap);
		if(0 == status_cercler)
		{
			TaskUpdateNextRunTime(current_task);
			status = BinHeapPush(scheduler->heap, current_task);
			if(success != status)
			{
				TaskDestroy(current_task);
				break;
			}
		}
		else
		{
			TaskDestroy(current_task);
		}
	}
	if(stop_run == scheduler->stop_flag)
	{
		return stop_run;
	}
	
	return status;
}


void SchedulerStop(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	scheduler->stop_flag = stop_run;
}

size_t SchedulerSize(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return BinHeapSize(scheduler->heap);
}

int SchedulerIsEmpty(scheduler_t *scheduler)
{
	assert(NULL != scheduler);
	return (0 == SchedulerSize(scheduler));
}

static int CompareTaskUid(const void *data, const void *param)
{
	UID_t *uid = (UID_t *)param;
	
	return TaskCompare((task_t *)data, *uid);
}


static int CompareNextRunTime(const void *data1, const void *data2)
{
	task_t *task1 = (task_t *)data1;
	task_t *task2 = (task_t *)data2;
	
	if (0 > difftime(TaskGetNextRunTime(task1), TaskGetNextRunTime(task2)))
	{
		return 1;
	}
	
	return -1;
}
