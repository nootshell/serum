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


#ifndef __LS_CORE_SETUP_MACROS_H
#define __LS_CORE_SETUP_MACROS_H




#define __LS_MKSTR(x)						#x
#define LS_MKSTR(x)							__LS_MKSTR(x)


#define __LS_MAYBE_EMPTY(x)					x ## 1
#define LS_MAYBE_EMPTY(x)					__LS_MAYBE_EMPTY(x)




#define __LS_NARG_S3						\
	20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, \
	 9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define __LS_NARG_S2(						\
	__00__, __01__, __02__, __03__, __04__, \
	__05__, __06__, __07__, __08__, __09__, \
	__10__, __11__, __12__, __13__, __14__, \
	__15__, __16__, __17__, __18__, __19__, \
	N, ...)									N

#define __LS_NARG_S1(...)					__LS_NARG_S2(__VA_ARGS__)

#define LS_NARG(...)						__LS_NARG_S1(__VA_ARGS__, __LS_NARG_S3)




#endif
