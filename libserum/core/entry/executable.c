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

#define FILE_PATH							"core/entry/executable.c"

#include "../stdincl.h"

#if (defined(ELF_INTERP))

#include "./main.h"
#include "../logging/log.h"
#include <stdlib.h>

#if (!LS_SELFTEST_STARTUP)
#	include "../self-test.h"
#endif


ID("entrypoint for executable library");


const char LS_ATTR_USED __attribute__((section(".interp"))) interp[] = ELF_INTERP;


void
interp_entry() {
	ls_log(LS_LOG_INFO, "Startup from executable library.");

	ls_nword_t result = 0, sub_result;

	if ((sub_result = lib_main_entry()) != 0) {
		result |= ((sub_result << 3) | 1);
	}

#if (!LS_SELFTEST_STARTUP)
	if (ls_selftest_all() != true) {
		result |= 2;
	}
#endif

	if ((sub_result = lib_main_exit()) != 0) {
		result |= ((sub_result << 6) | 4);
	}

	if (result) {
		ls_log(LS_LOG_WARNING, "Errors along the way, true exit code: %08X", result);
	}

	exit(!!result);
}

#else
LS_COMPILER_WARN("Library not executable: ELF interpreter unspecified.");
#endif
