#ifndef __clock_H__
#define __clock_H__
#include <time.h> /*clockid_t, size_t */
#include "misc.h" /* typdef sturct timespec */

typedef struct Clock Clock;


/**  
 * @brief Create new Clock 
 * @param[in] _clkId - specify the id of the system clock to be used.
 *  CLOCK_REALTIME, CLOCK_REALTIME_COARSE, CLOCK_MONOTONIC
 * @return Clock * - on success
 * @retval NULL on fail (_clockFunc was null or _period_ms was 0) 
 */
Clock* ClockCreate(clockid_t _clkId);

/**  
 * @brief Deallocate a previously allocated task
 * @param[in] _clock - task to be deallocated. 
 */
 void ClockDestroy(Clock * _clock);

timespec ClockGetCurrentTime(Clock * _clock);

timespec ClockAddTime(size_t _milliseconds, timespec _time);

int     ClockWakeMeUp(Clock * _clock, timespec _timeToSleep);

int LessThanCompereTimespec(timespec _a, timespec _b);



#endif /*__clock_H__*/
