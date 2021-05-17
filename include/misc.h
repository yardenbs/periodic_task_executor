#ifndef __MISC_H__
#define __MISC_H__

#define FALSE 0
#define TRUE !FALSE

typedef int bool_t;
typedef int	(*TaskFunction)(void * _context);
typedef void (*ElementDestroy)(void* _item);

typedef enum ResultCode {
	RESULT_SUCCESS,
	RESULT_NOT_INITIALIZED,
	RESULT_ALLOCATION_FAILED,
	RESULT_TASKFUNCTION_IS_NULL,
    RESULT_PERIOD_MS_IS_ZERO
} ResultCode;

typedef struct timespec timespec;


#endif /*__MISC_H__*/
