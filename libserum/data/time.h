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


#ifndef __LS_DATA_TIME_H
#define __LS_DATA_TIME_H




#include "../core/setup.h"
#include <time.h>




#ifdef __cplusplus
extern "C" {
#endif

	/*!
	 * \brief Gets the current time in nanoseconds.
	 *
	 * \return The current time in nanoseconds.
	 */
	LSAPI uint64_t ls_time_nanos(void);

	/*!
	 * \brief Gets the current time in seconds.
	 *
	 * \return The current time in seconds.
	 */
	LSAPI time_t ls_time_secs(void);

	/*!
	 * \brief Parses the specified \p time to a time structure.
	 *
	 * \param time The time.
	 * \param out_tm The time structure to populate.
	 *
	 * \return
	 *		`#LS_E_FAILURE` if the call to the platform's `localtime` failed.	\n
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t LS_ATTR_PURE ls_localtime(const time_t time, struct tm *const out_tm);

	/*!
	 * \brief Parses the current time to a time structure.
	 *
	 * \param out_tm The time structure to populate.
	 *
	 * \return
	 *		See `#ls_localtime`.
	 */
	LSAPI ls_result_t ls_localtime_now(struct tm *const out_tm);

	/*!
	 * \brief Gets the time in milliseconds from the specified timespec.
	 *
	 * \param ts The timespec to convert.
	 * \param out_millis The variable to store the resulting milliseconds in.
	 *
	 * \return
	 *		`#LS_E_NULL` if \p ts or \p out_millis is `NULL`.	\n
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t LS_ATTR_PURE ls_timespec_to_millis(const struct timespec *const restrict ts, uint64_t *const restrict out_millis);

	/*!
	 * \brief Gets the timespec from the specified time in milliseconds.
	 *
	 * \param millis The milliseconds to convert.
	 * \param out_ts The variable to store the resulting timespec in.
	 *
	 * \return
	 *		`#LS_E_NULL` if \p millis or \p out_ts is `NULL`.	\n
	 *		`#LS_E_SUCCESS` otherwise.
	 */
	LSAPI ls_result_t LS_ATTR_PURE ls_millis_to_timespec(const uint64_t millis, struct timespec *const out_ts);

	/*!
	 * \brief Reads the time stamp counter, which contains the number of cycles since the system reset.
	 *
	 * \return The value of the time stamp counter.
	 */
	LSAPI uint64_t ls_rdtsc(void);

	/*!
	 * \brief Sleeps for the specified number of nanoseconds.
	 *
	 * \param nanos The number of nanoseconds to sleep.
	 */
	LSAPI void ls_sleep_nanos(const uint64_t nanos);

	/*!
	 * \brief Sleeps for the specified number of milliseconds.
	 *
	 * \param millis The number of milliseconds to sleep.
	 */
	LSAPI void ls_sleep_millis(const uint32_t millis);

#ifdef __cplusplus
}
#endif




#endif
