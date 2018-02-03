/******************************************************************************
**                                                                           **
**   The MIT License                                                         **
**                                                                           **
**   Copyright 2017-2018 icecubetray                                         **
**                                                                           **
**   Permission is hereby granted, free of charge, to any person             **
**   obtaining a copy of this software and associated documentation files    **
**   (the "Software"), to deal in the Software without restriction,          **
**   including without limitation the rights to use, copy, modify, merge,    **
**   publish, distribute, sublicense, and/or sell copies of the Software,    **
**   and to permit persons to whom the Software is furnished to do so,       **
**   subject to the following conditions:                                    **
**                                                                           **
**   The above copyright notice and this permission notice shall be          **
**   included in all copies or substantial portions of the Software.         **
**                                                                           **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  **
**                                                                           **
******************************************************************************/



#ifndef __LS_RUNTIME_CONCURRENCY_MUTEX_H
#define __LS_RUNTIME_CONCURRENCY_MUTEX_H



#include "../../core/setup.h"
#include <time.h>

#if (LIBSERUM_DOXYGEN)
#	// Doxygen preprocessor.
#elif (LS_PTHREADS)
#	include <pthread.h>
#	define CONST_PTHREADS					const
#elif (LS_WTHREADS)
#	define CONST_WTHREADS					const
#else
#	error Unsupported threading API.
#endif

#ifndef CONST_PTHREADS
#	define CONST_PTHREADS
#endif

#ifndef CONST_WTHREADS
#	define CONST_WTHREADS
#endif

#if (LS_EXPORTING)
#	define LS_MUTEX_ACQUIRE_OR_ERROR(lockp)	if (ls_mutex_lock((lockp)) != LS_E_SUCCESS) { return LS_E_LOCK; }
#	define LS_MUTEX_RELEASE_OR_ERROR(lockp)	if (ls_mutex_unlock((lockp)) != LS_E_SUCCESS) { return LS_E_LOCK; }
#endif



typedef struct ls_mutex {
	//! \brief The (platform specific) locking object.
#if (LIBSERUM_DOXYGEN)
	platform_specific lock;
#elif (LS_PTHREADS)
	pthread_mutex_t lock;
#elif (LS_WTHREADS)
	void *lock;
#endif

	//! \brief Flags for the mutex. Currently only used to store its magic.
	ls_uint32_t __flags;
} ls_mutex_t;



#ifdef __cplusplus
extern "C" {
#endif

	/*! \brief Initializes a mutex.
	**
	**	\param mutex The mutex to initialize.
	**
	**	\return
	**		`#LS_E_NULL` if \p mutex is `NULL`;							<br/>
	**		`#LS_E_MAGIC` if the struct magic is already set;			<br/>
	**		`#LS_E_FAILURE` on failure;									<br/>
	**		`#LS_E_SUCCESS` on success;									<br/>
	**		`#LS_E_UNSUPPORTED` if the implementation is missing.
	*/
	LSAPI ls_result_t ls_mutex_init(ls_mutex_t *const mutex);

	/*!	\brief Clears a mutex.
	**
	**	\param mutex The mutex to clear.
	**
	**	\return
	**		`#LS_E_NULL` if \p mutex is `NULL`;							<br/>
	**		`#LS_E_MAGIC` if the struct magic is not set;				<br/>
	**		`#LS_E_FAILURE` on failure;									<br/>
	**		`#LS_E_SUCCESS` on success;									<br/>
	**		`#LS_E_UNSUPPORTED` if the implementation is missing.
	*/
	LSAPI ls_result_t ls_mutex_clear(ls_mutex_t *const mutex);

	/*!	\brief Locks a mutex.
	**
	**	If it is already locked, it will wait indefinitely until it is
	**	unlocked.
	**
	**	\param mutex The mutex to lock.
	**
	**	\return
	**		`#LS_E_NULL` if \p mutex is `NULL`;							<br/>
	**		`#LS_E_MAGIC` if the struct magic is not set;				<br/>
	**		`#LS_E_UNINITIALIZED` if \p mutex`->lock` is `NULL` (Windows);	<br/>
	**		`#LS_E_SUCCESS` on success;									<br/>
	**		`#LS_E_ABANDONED` if the lock was abandoned;				<br/>
	**		`#LS_E_FAILURE` on failure;									<br/>
	**		`#LS_E_UNSUPPORTED` if the implementation is missing.
	*/
	LSAPI ls_result_t ls_mutex_lock(CONST_WTHREADS ls_mutex_t *const mutex);

	/*!	\brief Locks a mutex.
	**
	**	Same as `#ls_mutex_lock`, except that this function will time-out
	**	after the specified \p timeout while waiting.
	**
	**	\param mutex The mutex to lock.
	**	\param timeout The time to wait before timing out.
	**
	**	\return
	**		`#LS_E_INVALID` if a timeout of zero was given;				<br/>
	**		`#LS_E_CONVERSION` if conversion of \p timeout failed;		<br/>
	**		`#LS_E_TIMEOUT` if it times out;							<br/>
	**		return values from `#ls_mutex_lock` otherwise.
	*/
	LSAPI ls_result_t ls_mutex_timedlock(CONST_WTHREADS ls_mutex_t *const mutex, const struct timespec timeout);

	//! \copydoc ls_mutex_timedlock
	LSAPI ls_result_t ls_mutex_timedlock_millis(CONST_WTHREADS ls_mutex_t *const mutex, const ls_uint64_t timeout_millis);

	/*!	\brief Unlocks a mutex.
	**
	**	\param mutex The mutex to unlock.
	**
	**	\return
	**		`#LS_E_NULL` if \p mutex is `NULL`;							<br/>
	**		`#LS_E_MAGIC` if the struct magic is not set;				<br/>
	**		`#LS_E_UNINITIALIZED` if \p mutex`->lock` is `NULL` (Windows);	<br/>
	**		`#LS_E_FAILURE` on failure;									<br/>
	**		`#LS_E_SUCCESS` on success;									<br/>
	**		`#LS_E_UNSUPPORTED` if the implementation is missing.
	*/
	LSAPI ls_result_t ls_mutex_unlock(CONST_WTHREADS ls_mutex_t *const mutex);

#ifdef __cplusplus
}
#endif



#endif
