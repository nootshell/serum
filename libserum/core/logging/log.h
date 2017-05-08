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

#ifndef __LS_CORE_LOGGING_LOG_H
#define __LS_CORE_LOGGING_LOG_H


#include "../stdincl.h"
#include "./ansi.h"
#include "../detect_platform.h"
#include <inttypes.h>


#if (LS_ARCH_BITS == 32)
#	define LS_PRIPTR						"08" PRIXPTR
#else
#	define LS_PRIPTR						"016" PRIXPTR
#endif


typedef enum ls_log_level {
	LS_LOG_SILENT = 0,
	LS_LOG_ERROR = 1,
	LS_LOG_WARNING = 2,
	LS_LOG_INFO = 3,
	LS_LOG_VERBOSE = 4,
	LS_LOG_DEBUG = 5
} ls_log_level_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI void ls_log(ls_log_level_t level, const char *fmt, ...);

#ifdef __cplusplus
}
#endif


#endif
