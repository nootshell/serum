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



#ifndef __LS_IO_LOG_H
#define __LS_IO_LOG_H



#include "../core/setup.h"

#include <stdio.h>



// log defs

#define LS_LOG_CLEAR_CLOSE_STD				0x0001
#define LS_LOG_CLEAR_CLOSE_ERR				0x0002



typedef enum ls_log_level {
	LS_LOG_LEVEL_ERROR = 1,
	LS_LOG_LEVEL_WARNING = 2,
	LS_LOG_LEVEL_INFO = 3,
	LS_LOG_LEVEL_VERBOSE = 4,
	LS_LOG_LEVEL_DEBUG = 5
} ls_log_level_t;

typedef struct ls_log {
	FILE *fstd;
	FILE *ferr;
	ls_uint32_t flags;
	ls_log_level_t level;
} ls_log_t;
#define a sizeof(ls_log_t)



#endif
