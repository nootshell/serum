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


#ifndef __LS_CORE_SETUP_H
#define __LS_CORE_SETUP_H




#include <stdlib.h>
#include <errno.h>

#include "./setup/types.h"
#include "./setup/platform-setup.h"

#include "./result.h"
#include "./signatures.h"

#if (LS_EXPORTING)
#	include "./setup/fileid.h"
#	include "./setup/magic.h"
#	include "./setup/bitops.h"
#endif




#if (!LS_WINDOWS || LS_THREADING_PTHREADS)
#	define LS_PTHREADS						1
#elif (LS_WINDOWS)
#	define LS_WTHREADS						1
#endif

#ifndef LS_PTHREADS
#	define LS_PTHREADS						0
#endif

#ifndef LS_WTHREADS
#	define LS_WTHREADS						0
#endif




#if (LS_EXPORTING)
#	include "../io/log.h"
#
#	if (LS_DEBUG)
#		define ls_debug(str)				ls_log_write(NULL, LS_LOG_LEVEL_DEBUG, "%s:%u " str, __func__, __LINE__)
#		define ls_debugf(fmt, ...)			ls_log_write(NULL, LS_LOG_LEVEL_DEBUG, "%s:%u " fmt, __func__, __LINE__, __VA_ARGS__)
#	else
#		define ls_debug(str)
#		define ls_debugf(fmt, ...)
#	endif
#endif




#endif
