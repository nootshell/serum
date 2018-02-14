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


#ifndef __LS_CORE_SETUP_MAGIC_H
#define __LS_CORE_SETUP_MAGIC_H




#include "../setup.h"




#define LS_MAGIC8							0xED

#define LS_MAGIC32							(LS_MAGIC8 << 24)
#define LS_MAGIC32_STRIP(flags)				((flags) & 0x00FFFFFF)
#define LS_MAGIC32_SET(flags)				(LS_MAGIC32_STRIP(flags) | LS_MAGIC32)
#define LS_MAGIC32_VALID(flags)				(((flags) & 0xFF000000) == LS_MAGIC32)

#define LS_MAGIC64							(LS_MAGIC8 << 56)
#define LS_MAGIC64_STRIP(flags)				((flags) & 0x00FFFFFFFFFFFFFF)
#define LS_MAGIC64_SET(flags)				(LS_MAGIC64_STRIP(flags) | LS_MAGIC64)
#define LS_MAGIC64_VALID(flags)				(((flags) & 0xFF00000000000000) == LS_MAGIC64)




#if (LS_EXPORTING)
#	if (!LS_NO_WAND_FLICKING)
#		define __LS_MAGIC32_FLICK_W(n, f)	if (n LS_MAGIC32_VALID(f)) { return LS_E_MAGIC; }
#		define __LS_MAGIC64_FLICK_W(n, f)	if (n LS_MAGIC64_VALID(f)) { return LS_E_MAGIC; }
#	else
#		define __LS_MAGIC32_FLICK_W(n, f)
#		define __LS_MAGIC64_FLICK_W(n, f)
#	endif

#	define LS_MAGIC32_VALID_OR_ERROR(f)		__LS_MAGIC32_FLICK_W(!, f);
#	define LS_MAGIC32_INVALID_OR_ERROR(f)	__LS_MAGIC32_FLICK_W(, f);
#	define LS_MAGIC64_VALID_OR_ERROR(f)		__LS_MAGIC64_FLICK_W(!, f);
#	define LS_MAGIC64_INVALID_OR_ERROR(f)	__LS_MAGIC64_FLICK_W(, f);
#endif




#endif
