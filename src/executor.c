#include <stdlib.h> /*malloc , free, NULL */
#include "executor.h"
#include "clock.h"
#include "heap.h"
#include "task.h"
#define VECTOR_CAPACITY 20
#define VECTOR_BLOCK 10

/*TODO: add heap to struct and change addTaskFunction to add to vector or heap */
struct Executor
{
    Clock *     m_clock;
    Vector *    m_vector;
    bool_t      m_isRunning;
};

static int UpdateTime(Task * _task, size_t _index, timespec * _time);
static Heap * InitSchedule(Executor * _executor);
static void ReScheduleTask(Heap * _heap, Task * _task);
 static void SleepRunAndReschedule(Task * _task, Clock * _clock, Heap * _heap);

Executor* ExecutorCreate(clockid_t _clkId)
{
    Clock * clk;
    Vector * vec;
    Executor * exec;

    if (!(clk = ClockCreate(_clkId)))
    {
        return NULL;
    }
    if(!(vec = VectorCreate(VECTOR_CAPACITY,VECTOR_BLOCK)))
    {
        free(clk);
        return NULL;
    }
    if(!(exec = (Executor *)calloc(1, sizeof(Executor))))
    {
        free(clk);
        free(vec);
        return NULL;
    }
    
    exec->m_clock = clk;
    exec->m_vector = vec; 

    return exec; 
}
 void ExecutorDestroy(Executor * _executor)
 {
     if (_executor)
     {
        VectorDestroy(&_executor->m_vector,(ElementDestroy)TaskDestroy);
        ClockDestroy(_executor->m_clock);
        free(_executor);   
     }
 }

 ResultCode ExecutorAdd(Executor * _executor,
                          TaskFunction _taskFunc,
                          void * _context,
                          size_t _period_ms)
{
    Task * task;
    timespec time;
    if(!_executor)
    {
        return RESULT_NOT_INITIALIZED;
    }
    if(!_taskFunc)
    {
        return RESULT_TASKFUNCTION_IS_NULL;
    }
    if(!_period_ms)
    {
        return RESULT_PERIOD_MS_IS_ZERO;
    }

    task = TaskCreate(_taskFunc,_context,_period_ms);
    if(VECTOR_SUCCESS != VectorAppend(_executor->m_vector, task))
    {
        return RESULT_ALLOCATION_FAILED;
    }
    return RESULT_SUCCESS;
}  

 size_t ExecutorRun(Executor * _executor)
 {
     size_t cycles = 0;
     Task * task = NULL;
     Heap * minHeap = NULL;
    /*TODO: CHECK IF VECTOR IS EMPTY */
     if (_executor && (minHeap = InitSchedule(_executor)))
     {
         while (_executor->m_isRunning)
         {
            task = (Task *)HeapExtract(minHeap);
            if (task)
            {
              SleepRunAndReschedule(task,_executor->m_clock, minHeap);
              cycles++; 
            }
            else
            {
                _executor->m_isRunning = FALSE;
            }
         }

         HeapDestroy(&minHeap);    
     }  

     return cycles;
 }
static void ReScheduleTask(Heap * _heap, Task * _task)
{
    timespec time;

    time = TaskGetExecutedTime(_task);
    UpdateTime(_task, 0, & time);
    HeapInsert(_heap,_task);
}


 size_t ExecutorPause(Executor * _executor)
 {
     size_t remainingTasks = 0;

     if(_executor)
     {
         _executor->m_isRunning = FALSE;
         remainingTasks = VectorSize(_executor->m_vector);
     }

     return remainingTasks;
 }

 static void SleepRunAndReschedule(Task * _task, Clock * _clock, Heap * _heap)
 {
     ClockWakeMeUp(_clock, TaskGetExecutedTime(_task));

    if(TaskRun(_task) == 1)
    {
        ReScheduleTask(_heap, _task);
    }
    else
    {
        TaskDestroy(_task);
    }  
 }


static Heap * InitSchedule(Executor * _executor)
{
    Heap * minHeap;
    timespec currentTime = ClockGetCurrentTime(_executor->m_clock);

    VectorForEach(_executor->m_vector,(VectorElementAction)UpdateTime,(void*)&currentTime);
    minHeap = HeapBuild(_executor->m_vector,(LessThanComparator)LessThanTaskTime);
    _executor->m_isRunning = minHeap != NULL;

    return minHeap;
}

static int UpdateTime(Task * _task, size_t _index, timespec * _time)
{
    TaskUpdateExecutedTime(_task,ClockAddTime(TaskGetPeriodTime(_task), *_time));
    return 1;  
}
