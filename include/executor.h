#ifndef __EXECUTOR_H__
#define __EXECUTOR_H__
#include <sys/types.h> /*clockid_t */
#include "misc.h" /*TaskFunction, ResultCode*/

/** 
 * @brief Create a periodic task executor 
 * that allow the user to enqueue tasks to be executed
 * @author Yarden Ben Shimon 
 */ 

typedef struct Executor Executor;

/**  
 * @brief create a periodic executor that will measure time using giving clock.
 * @param[in] _clkId - specify the id of the system clock to be used.
 *  CLOCK_REALTIME, CLOCK_REALTIME_COARSE, CLOCK_MONOTONIC
 * @return Executor * - on success
 * @retval NULL on fail 
 */
Executor* ExecutorCreate(clockid_t _clkId);

/**  
 * @brief Deallocate a previously allocated executor
 * @param[in] _executor - executor to be deallocated. 
 *  On success will be nulled.
 */
 void ExecutorDestroy(Executor * _executor);

/**  
 * @brief Adding to the executor 
 * @param[in] _executor - Executor to insert task.
 * @param[in] _taskFunc - user provided function pointer. can't be null
 * @param[in] _context - the task function is called with _context as only parameter 
 * @param[in] _period_ms -is the period of recurrence in milliseconds. can't be 0 
 * @return success or error code,
 * @retval RESULT_SUCCESS  on success
 * @retval RESULT_NOT_INITIALIZED  error, executor not initialized.
 * @retval RESULT_ALLOCATION_FAILED error, executor could not allocate new task
 * @retval RESULT_TASKFUNCTION_IS_NULL error, if taskFunction is null
 * @retval RESULT_PERIOD_MS_IS_ZERO error, if period_ms is 0  
 */
ResultCode ExecutorAdd(Executor * _executor,
                          TaskFunction _taskFunc,
                          void * _context,
                          size_t _period_ms);

/**  
 * @brief- Start running the tasks previously added to the executor,
 *         Or resume a previously paused executor. 
 * @param[in] _executor - executor to run.
 * @return number of execution cycles performed.
 */
 size_t ExecutorRun(Executor * _executor);

/**  
 * @brief- pause running executor.
 * @param[in] _executor - executor to pause.
 * @return number of tasks remaining inside the executor.
 */
 size_t ExecutorPause(Executor * _executor);

#endif /*__EXECUTOR_H__*/
