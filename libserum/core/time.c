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

#define FILE_PATH							"core/time.c"

#include <time.h>
#include "./time.h"
#include "./detect_os.h"
#include "./detect_platform.h"


ID("timing functionality");


uint64_t
ls_rdtsc() {
#if (LS_ARM)
#	if defined(LS_ARM_VERSION)
# 		if (LS_ARCH_ARM_VERSION == 8)
	uint64_t r = 0;
	asm volatile ("mrs %0, cntvct_e10" : "=r"(r));
	return r;
#		elif (LS_ARCH_ARM_VERSION >= 6)
	uint32_t r = 0;
	asm volatile ("mrc p15, 0, %0, c9, c14, 0" : "=r"(r));
	if (HAS_FLAG(r, 0x00000001)) {
		asm volatile ("mrc, p15, 0, %0, c9, c12, 1", "=r"(r));
		if (HAS_FLAG(r, 0x80000000)) {
			asm volatile ("mrc p15, 0, %0, c9, c13, 0" : "=r"(r));
			return (((uint64_t)r) << 6);
		}
	}
#		endif
#	else
#		error RDTSC unsupported.
#	endif
#else
#	if (LS_INTRINSICS)
	return __rdtsc();
#	else
	uint32_t hi, lo;
	asm volatile ("rdtscp\n"
				  "movl %%edx, %0\n"
				  "movl %%eax, %1\n"
				  "cpuid"
				  : "=r"(hi), "=r"(lo)
				  :
				  : "%rax", "%rbx", "%rcx", "%rdx");
	return ((((uint64_t)hi) << 32) | lo);
#	endif
#endif

	// Failsafe
	return ls_nanos();
}


uint64_t
ls_nanos() {
#if (LS_WINDOWS)
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	return (((((uint64_t)ft.dwHighDateTime << 32) | ft.dwLowDateTime) / 10) - 0x295E9648864000);
#else
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
		return (ts.tv_sec * 1000000000) + ts.tv_nsec;
	}
#endif

	// Failsafe
	return 0;
}
