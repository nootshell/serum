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

#ifndef __LS_DEBUG_LOG_H
#define __LS_DEBUG_LOG_H


#include "../core/integers.h"
#include "../core/lsapi.h"
#include "../core/identification.h"

#include "../core/errstr.h"

#include <stdarg.h>


#ifndef FILE_PATH
#define FILE_PATH							"unspec_file"
#endif


#define ls_log(s)							_ls_log(__func__, FILE_PATH, __LINE__, (s))
#define ls_logf(fmt, ...)					_ls_logf(__func__, FILE_PATH, __LINE__, (fmt), __VA_ARGS__)


#define ls_log_e(s)							ls_log("[err] " s)
#define ls_log_w(s)							ls_log("[wrn] " s)
#define ls_log_i(s)							ls_log("[inf] " s)
#define ls_log_v(s)							ls_log("[ver] " s)

#if (DEBUG)
#	define ls_log_d(s)						ls_log("[dbg] " s)
#	define ls_logf_d(fmt, ...)				ls_logf("[dbg] " fmt, __VA_ARGS__)
#else
#	define ls_log_d(s)
#	define ls_logf_d(fmt, ...)
#endif

#define ls_lognull()						_ls_log(NULL, NULL, 0, NULL);


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI void _ls_log(const char *func, const char *file, const uint32_t line, const char *const str);
	LSAPI void _ls_logf(const char *func, const char *file, const uint32_t line, const char *const fmt, ...);

#ifdef __cplusplus
}
#endif


#endif
