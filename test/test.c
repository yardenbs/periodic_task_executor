#include <time.h>
#include <sys/unistd.h>
#include <sys/types.h>
#include "executor.h"
#include <stdio.h>

int printhello(void *);
int printbye(void *);
int stop(void *);
int printA(void * _null);

int main()
{
	Executor *  ex = ExecutorCreate(CLOCK_MONOTONIC);
	int size = 2;
	size_t many;
	ExecutorAdd(ex,printbye,NULL,7000);
	ExecutorAdd(ex,printhello,&size,3000);
	ExecutorAdd(ex,stop,(void*)ex,5000);
	many = ExecutorRun(ex);
	printf("after ExecutorRun finish: %ld tasks executed so far\n", many);
	ExecutorAdd(ex,printA,NULL,3000);
	printf("in main: we added printA function\n");
	ExecutorRun(ex);

	ExecutorDestroy(ex);

	return 0;
}



int printhello(void * _size)
{
	printf("in printhello:hello\n");
	--*(int*)_size;

	return *(int*)_size;
}

int stop(void * _ex)
{
	size_t size = ExecutorPause((Executor*)_ex);

	printf("in stop:still reaming %ld tasks\n", size);

	return 0;
}

int printbye(void * _null)
{
	printf("in printbye: bye\n");
	return 0;
}
int printA(void * _null)
{
	printf("in printA: A\n");
	return 0;
}
