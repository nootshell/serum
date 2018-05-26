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


#ifndef __LS_CORE_SETUP_DEBUG_H
#define __LS_CORE_SETUP_DEBUG_H
#if (LS_EXPORTING && LS_DEBUG)




#include "../../io/log.h"




#define ls_debug(str)						ls_log_write(NULL, LS_LOG_LEVEL_DEBUG, "%s:%u " str, __func__, __LINE__)
#define ls_debugf(fmt, ...)					ls_log_write(NULL, LS_LOG_LEVEL_DEBUG, "%s:%u " fmt, __func__, __LINE__, __VA_ARGS__)
#define ls_debugfe(fmt, ...)				ls_log_write(NULL, LS_LOG_LEVEL_ERROR, "%s:%u " fmt, __func__, __LINE__, __VA_ARGS__)

#define ls_error(err)						ls_log_write(NULL, LS_LOG_LEVEL_ERROR, "%s:%u [%u] %s", __func__, __LINE__, (err), ls_result_string((err)))
#define return_e(err)						ls_error(err); return (err)




#else
#	define ls_debug(x)
#	define ls_debugf(x, ...)
#	define ls_debugfe(x, ...)
#	define ls_error(x)
#	define return_e(err)					return (err)
#endif
#endif
