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

	LSAPI uint64_t ls_time_nanos();
	LSAPI time_t ls_time_secs();

	LSAPI ls_result_t LS_ATTR_PURE ls_localtime(const time_t time, struct tm *const out_tm);
	LSAPI ls_result_t ls_localtime_now(struct tm *const out_tm);

	LSAPI ls_result_t LS_ATTR_PURE ls_timespec_to_millis(const struct timespec *const restrict ts, uint64_t *const restrict out_millis);
	LSAPI ls_result_t LS_ATTR_PURE ls_millis_to_timespec(const uint64_t millis, struct timespec *const out_ts);

	LSAPI uint64_t ls_rdtsc();

#ifdef __cplusplus
}
#endif




#endif
