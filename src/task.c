#include <stdlib.h> /*malloc , free NULL */
#include <time.h> /*sturct timespec */
#include "task.h"
#include "misc.h" /* TaskFunction*/
#include "clock.h" /* LessThanCompereTimespec */

struct Task
{
    TaskFunction m_taskFunc;
    void * m_context;;
    size_t m_period_ms;
    timespec  m_executeTime;
};

Task* TaskCreate(TaskFunction _taskFunc,void * _context,size_t _period_ms)
{
    Task * task = (Task*)malloc(sizeof(Task));
    if(task)
    {
        task->m_taskFunc = _taskFunc;
        task->m_context = _context;
        task->m_period_ms = _period_ms;
        task->m_executeTime = (timespec){0,0};
    }
    return task;
}

 void TaskDestroy(Task * _task)
 {
     free(_task);
 }

 int TaskRun(Task * _task)
 {
     return (_task) ? _task->m_taskFunc(_task->m_context) : -1;
 }

 ResultCode TaskUpdateExecutedTime(Task * _task, timespec _updateTime)
 {
     if (!_task)
     {
         return RESULT_NOT_INITIALIZED;
     }
     _task->m_executeTime = _updateTime;

     return RESULT_SUCCESS;
 }

 timespec TaskGetExecutedTime(const Task * _task)
 {
    return  _task->m_executeTime;
 }

 size_t TaskGetPeriodTime(const Task * _task)
 {
     return _task->m_period_ms;
 }

int LessThanTaskTime(const Task * _task1,const Task * _task2)
 {
     return  LessThanCompereTimespec(_task1->m_executeTime, _task2->m_executeTime);
 }
