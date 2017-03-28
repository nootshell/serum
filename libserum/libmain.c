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

#define FILE_PATH							"entrypoint.c"

#include "./core/detect_compiler.h"
#include "./core/detect_endianness.h"
#include "./debug/log.h"
#include <stdint.h>
#include <stdlib.h>

#if (defined(DEBUG) && DEBUG)
#include "./core/intrinsics.h"
#include "./core/detect.h"
#include "./core/info.h"
#endif


#if ((defined(DEBUG) && DEBUG) && defined(__has_include))
#	if (__has_include(<mcheck.h>))
#		include <mcheck.h>

void
static ls_mcheck_abort(enum mcheck_status status) {
	switch (status) {
		case MCHECK_OK:
			return;
		case MCHECK_FREE:
			ls_log_e("Block freed twice.");
			return;
		case MCHECK_HEAD:
			ls_log_e("Memory before the block was clobbered.");
			return;
		case MCHECK_TAIL:
			ls_log_e("Memory after the block was clobbered.");
			return;
	}
}

int
static ls_hook_mcheck() {
	return mcheck(ls_mcheck_abort);
}

#	else
#		define ls_hook_mcheck()				-1
LS_COMPILER_WARN("Unable to include mcheck.h");
#	endif
#else
#	define ls_hook_mcheck()					-1
#endif


int
static LS_ATTR_CONSTRUCTOR libmain() {
#if (defined(DEBUG) && DEBUG)
	ls_log_d("Compilation environment: " LS_COMPILATION_ENVIRONMENT);
	if (ls_hook_mcheck() == 0) {
		ls_log_d("Successfully installed mcheck hooks.");
	} else {
		ls_log_e("Failed to install mcheck hooks (not linked with -lmcheck?).");
	}
#endif

	/* Check endianness. */ {
		uint16_t e_val = 0x6927;
		uint8_t e_val_f = ((uint8_t*)&e_val)[0];

#if (LS_BIG_ENDIAN)
#	define E_VAL_F_VALID					0x69
#else
#	define E_VAL_F_VALID					0x27
#endif

		if (e_val_f != E_VAL_F_VALID) {
			ls_log_e("Compile-time/run-time endianness mismatch - aborting.");
			abort();
		} else {
			ls_log_d("Compile-time/run-time endianness match.");
		}
	}

	ls_lognull();
	return 0;
}

int
static LS_ATTR_DESTRUCTOR libniam() {
	return 0;
}


/*
** Special treatment section.
*/

// Guess who's in here.
#if (LS_MSC)
#	include <Windows.h>

int
WINAPI DllMain(HINSTANCE handle, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		return (libmain() == 0);
	}
	if (reason == DLL_PROCESS_DETACH) {
		return (libniam() == 0);
	}
}
#endif
