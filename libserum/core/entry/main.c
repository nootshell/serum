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

#define FILE_PATH							"core/entry/main.c"

#include "./main.h"
#include "./debug.h"
#include "../exit.h"
#include "../info.h"
#include "../logging/log.h"

#if (defined(LS_SELFTEST_STARTUP))
#	include "../self-test.h"
#endif


ID("entry/exit routines");


ls_nword_t
lib_main_entry() {
	ls_nword_t result = 0;

#if (LS_VERBOSE_STARTUP)
	ls_log(LS_LOG_DEBUG, "Git: from branch %s, commit %s", ls_info_git_branch(), ls_info_git_commit());
	ls_log(LS_LOG_DEBUG, "Compilation environment: %s", ls_info_compilation_environment());
	ls_log(LS_LOG_DEBUG, "Compilation options:\n%s", ls_info_compilation_options());
#endif

	if (ls_hook_mcheck() != 0) {
		result |= 1;
	}

	/* Check endianness. */ {
		uint16_t e_val = 0x6927;

		#if (LS_BIG_ENDIAN)
		#	define E_VAL_VALID						0x69
		#else
		#	define E_VAL_VALID						0x27
		#endif

		if (((uint8_t*)&e_val)[0] != E_VAL_VALID) {
			ls_fatal(LS_LOG_ERROR, "Compile-time/run-time endianness mismatch - aborting.");
		} else {
#if (LS_VERBOSE_STARTUP)
			ls_log(LS_LOG_DEBUG, "Compile-time/run-time endianness match.");
#endif
		}
	}

#if (LS_SELFTEST && LS_SELFTEST_STARTUP)
	if (!ls_selftest_all()) {
		result |= 2;
	}
#endif

	return result;
}


ls_nword_t
lib_main_exit() {
	return 0;
}
