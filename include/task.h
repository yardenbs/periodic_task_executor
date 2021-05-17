#ifndef __TASK_H__
#define __TASK_H__
#include "misc.h" /*TaskFunction */

typedef struct Task Task;

/**  
 * @brief Create new Task 
 * @param[in] _taskFunc - user provided function pointer. can't be null
 * @param[in] _context - the task function is called with _context as only parameter 
 * @param[in] _period_ms -is the period of recurrence in milliseconds. can't be 0 
 * @return Task * - on success
 * @retval NULL on fail (_taskFunc was null or _period_ms was 0) 
 */
Task* TaskCreate(TaskFunction _taskFunc,void * _context,size_t _period_ms);

/**  
 * @brief Deallocate a previously allocated task
 * @param[in] _task - task to be deallocated. 
 *  On success will be nulled.
 */
 void TaskDestroy(Task * _task);
/**  
 * @brief Deallocate a previously allocated task
 * @param[in] _task - task to be execute. 
 * @return - 
 * @retval 1 to rescheduled 
 * @retval 0 to not rescheduled
 * @retval -1 error _task was null 
 */
 int TaskRun(Task * _task);

 ResultCode TaskUpdateExecutedTime(Task * _task, timespec _updateTime);
 timespec TaskGetExecutedTime(const Task * _task);
 size_t TaskGetPeriodTime(const Task * _task);
 int LessThanTaskTime(const Task * _task1,const Task * _task2);

#endif /*__TASK_H__*/
