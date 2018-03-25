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


#include "./time.h"




FILEID("Time-related routines.");




uint64_t
ls_time_nanos() {
#if (LS_MSC || LS_MINGW)
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return (((((uint64_t)ft.dwHighDateTime << LS_BITS_DWORD) | ft.dwLowDateTime) / 10) - 0x295E9648864000);
#else
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) != 0) {
		return 0;
	}
	return ((ts.tv_sec * 1000000000) + ts.tv_nsec);
#endif
}

time_t
ls_time_secs() {
	return time(NULL);
}




ls_result_t
ls_localtime(const time_t time, struct tm *const out_tm) {
#if (LS_MSC || LS_MINGW)
	if (localtime_s(out_tm, &time) != 0) {
		return 1;
	}
#else
	if (localtime_r(&time, out_tm) == NULL) {
		return 1;
	}
#endif

	return 0;
}


ls_result_t
ls_localtime_now(struct tm *const out_tm) {
	return ls_localtime(
		ls_time_secs(),
		out_tm
	);
}




ls_result_t
ls_timespec_to_millis(const struct timespec *const restrict ts, uint64_t *const restrict out_millis) {
	if (ts == NULL || out_millis == NULL) {
		return LS_E_NULL;
	}

	*out_millis = ((ts->tv_sec * 1000) + (ts->tv_nsec / 1000000));
	return LS_E_SUCCESS;
}


ls_result_t
ls_millis_to_timespec(const uint64_t millis, struct timespec *const out_ts) {
	if (out_ts == NULL) {
		return LS_E_NULL;
	}

	if (millis == 0) {
		out_ts->tv_sec = out_ts->tv_nsec = 0;
	} else {
		out_ts->tv_sec = (millis / 1000);
		out_ts->tv_nsec = (long int)((millis - out_ts->tv_sec) * 1000000);
	}

	return LS_E_SUCCESS;
}




uint64_t
ls_rdtsc() {
#if (LS_INTRINSICS_GOT_RDTSC)
	return LS_RDTSC();
#else
	uint64_t tsc = 0;

#	if ((LS_GCC || LS_LLVM) && (LS_X86 || LS_X64))
	asm volatile (
		"rdtsc\n\t"
		"shl $32, %%rdx\n\t"
		"or %%rdx, %0"
		: "=a" (tsc)
		:
		: "rdx"
	);
#	elif ((LS_MSC || LS_MINGW) && LS_X86)
	__asm {
		rdtsc

#		if (LS_BIG_ENDIAN)
		mov dword ptr [tsc + 0], edx
		mov dword ptr [tsc + 4], eax
#		else
		mov dword ptr [tsc + 4], edx
		mov dword ptr [tsc + 0], eax
#		endif
	};
#	elif (!defined(LS_IGNORE_RDTSC))
#		error Missing RDTSC implementation.
#	endif

	return tsc;
#endif
}
