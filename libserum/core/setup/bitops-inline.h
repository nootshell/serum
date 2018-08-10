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


#ifndef __LS_CORE_SETUP_BITOPS_INLINE_H
#define __LS_CORE_SETUP_BITOPS_INLINE_H




#include "./types.h"
#include "./platform-setup.h"




#if (LS_BITS_BYTE == 8)
#	undef __MKROTATE
#	define __MKROTATE(type_ret, name, bits, op1, op2)										\
		static LS_FORCE_INLINE																\
		type_ret name(type_ret value, unsigned int rotate) {								\
			return (type_ret)((value op1 rotate) | (value op2 (-rotate & ((bits) - 1))));	\
		}

	__MKROTATE( uint8_t,  __ls_rotl8,  LS_BITS_BYTE, <<, >>)
	__MKROTATE(uint16_t, __ls_rotl16,  LS_BITS_WORD, <<, >>)
	__MKROTATE(uint32_t, __ls_rotl32, LS_BITS_DWORD, <<, >>)
	__MKROTATE(uint64_t, __ls_rotl64, LS_BITS_QWORD, <<, >>)

	__MKROTATE( uint8_t,  __ls_rotr8,  LS_BITS_BYTE, >>, <<)
	__MKROTATE(uint16_t, __ls_rotr16,  LS_BITS_WORD, >>, <<)
	__MKROTATE(uint32_t, __ls_rotr32, LS_BITS_DWORD, >>, <<)
	__MKROTATE(uint64_t, __ls_rotr64, LS_BITS_QWORD, >>, <<)

#	undef __MKROTATE
#else
#	error Missing rotate fallbacks.
#endif




#endif
