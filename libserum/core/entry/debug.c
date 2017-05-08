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

#define FILE_PATH							"core/entry/debug.c"

#include "./debug.h"
#include "../logging/log.h"
#include <inttypes.h>


ID("debug hooking");


#if (DEBUG && LS_HAVE_CHECK_INCLUDE)
#	if (LS_CHECK_INCLUDE(mcheck.h))
#		define WE_HAVE_MCHECK
#		include <mcheck.h>

void
ls_mcheck_abort(enum mcheck_status status) {
	switch (status) {
		case MCHECK_OK:
			return;
		case MCHECK_FREE:
			ls_log(LOG_LEVEL_ERR, "Block freed twice.");
			return;
		case MCHECK_HEAD:
			ls_log(LOG_LEVEL_ERR, "Memory before the block was clobbered.");
			return;
		case MCHECK_TAIL:
			ls_log(LOG_LEVEL_ERR, "Memory after the block was clobbered.");
			return;
		default:
			ls_log(LOG_LEVEL_ERR, "Mcheck invoked abort, but an unknown status was passed to the abort function.");
			return;
	}
}

#	else
LS_COMPILER_WARN("mcheck.h not found");
#	endif
#endif

int
ls_hook_mcheck() {
#if (defined(WE_HAVE_MCHECK))
	ls_log("Trying to install mcheck hooks..."); // FIXME: weird ass segfault if we don't log before we hook mcheck
	int result = mcheck(ls_mcheck_abort);

	if (result == 0) {
		ls_log(LOG_LEVEL_DBG, "Installed successfully, abort function is located at address 0x%" LS_PRIPTR ".", (uintptr_t)ls_mcheck_abort);
	} else {
		ls_log(LOG_LEVEL_ERR, "Failed to install hooks.");
	}

	return result;
#else
	return -1;
#endif
}
