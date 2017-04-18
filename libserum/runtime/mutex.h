/*******************************************************************************
**                                                                            **
**   The MIT License                                                          **
**                                                                            **
**   Copyright 2017 icecubetray                                               **
**                                                                            **
**   Permission is hereby granted, free of charge, to any person              **
**   obtaining a copy of this software and associated documentation files     **
**   (the "Software"), to deal in the Software without restriction,           **
**   including without limitation the rights to use, copy, modify, merge,     **
**   publish, distribute, sublicense, and/or sell copies of the Software,     **
**   and to permit persons to whom the Software is furnished to do so,        **
**   subject to the following conditions:                                     **
**                                                                            **
**   The above copyright notice and this permission notice shall be           **
**   included in all copies or substantial portions of the Software.          **
**                                                                            **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   **
**                                                                            **
********************************************************************************
**
**  Notes:
**    -
**
*/

#ifndef __LS_RUNTIME_MUTEX_H
#define __LS_RUNTIME_MUTEX_H


#define __NO_MUTEX_API_STR					"No mutex API selected."


#include "../core/stdincl.h"

#if (!LS_WINDOWS)
#	include <pthread.h>
#	define LS_USING_PMUTEX					1
#else
#	define LS_USING_WNMUTEX					1
#endif

#ifndef LS_USING_PMUTEX
#define LS_USING_PMUTEX						0
#endif

#ifndef LS_USING_WNMUTEX
#define LS_USING_WNMUTEX					0
#endif


typedef struct ls_mutex {
#if (LS_USING_PMUTEX)
	pthread_mutex_t lock;
#elif (LS_USING_WNMUTEX)
	void *lock;
#else
	LS_COMPILER_WARN(__NO_MUTEX_API_STR);
#endif
} ls_mutex_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_mutex_init(ls_mutex_t *mutex);
	LSAPI ls_result_t ls_mutex_clear(ls_mutex_t *mutex);

	LSAPI ls_result_t ls_mutex_lock_ex(ls_mutex_t *mutex, uint32_t timeout);
	LSAPI ls_result_t ls_mutex_lock(ls_mutex_t *mutex);
	LSAPI ls_result_t ls_mutex_unlock(ls_mutex_t *mutex);

#ifdef __cplusplus
}
#endif


#endif
