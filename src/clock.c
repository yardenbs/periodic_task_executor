#include <time.h> /* clockid_t*/
#include <stdlib.h> /*malloc calloc NULL */
#include "clock.h"

struct Clock
{
    clockid_t m_clkId;
};

Clock* ClockCreate(clockid_t _clkId)
{
    Clock * clk;

    if ((_clkId != CLOCK_REALTIME &&
        _clkId != CLOCK_REALTIME_COARSE &&  
        _clkId != CLOCK_MONOTONIC) 
        || 
       !(clk = (Clock*) malloc(sizeof(Clock))))
    {
        return NULL;
    }

    clk->m_clkId = _clkId;

    return clk;
} 
void ClockDestroy(Clock * _clock)
{
    free(_clock);
}


timespec ClockGetCurrentTime(Clock * _clock)
{
    timespec currentTime = {0,0};

    if (_clock)
    {
        clock_gettime(_clock->m_clkId, &currentTime);    
    }

    return currentTime;
}


timespec ClockAddTime(size_t _milliseconds, timespec _time)
{
    _time.tv_sec += _milliseconds/1000;
    _time.tv_nsec += (_milliseconds * 1000000) % 1000000000;

    if(_time.tv_nsec >= 1000000000) 
    {
        _time.tv_nsec -= 1000000000;
        _time.tv_sec++;
    }

    return _time;
}
int ClockWakeMeUp(Clock * _clock, timespec _timeToSleep)
{
    if (_clock)
    {
       return clock_nanosleep(_clock->m_clkId, TIMER_ABSTIME, &_timeToSleep, NULL);     
    }
    return -1;
}


 int LessThanCompereTimespec(timespec _a, timespec _b)
 {
     return (_a.tv_sec != _b.tv_sec) ? _a.tv_sec - _b.tv_sec :
        _a.tv_nsec - _b.tv_nsec;
 }

