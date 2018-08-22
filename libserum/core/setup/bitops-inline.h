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




//! \brief Rotates the 8-bit \p value \p n bits to the left.
static LS_FORCE_INLINE
uint8_t __ls_rotl8(uint8_t value, unsigned int n) {
	return (uint8_t)((value << n) | (value >> (-n & ((LS_BITS_BYTE) - 1))));
}

//! \brief Rotates the 16-bit \p value \p n bits to the left.
static LS_FORCE_INLINE
uint16_t __ls_rotl16(uint16_t value, unsigned int n) {
	return (uint16_t)((value << n) | (value >> (-n & ((LS_BITS_WORD) - 1))));
}

//! \brief Rotates the 32-bit \p value \p n bits to the left.
static LS_FORCE_INLINE
uint32_t __ls_rotl32(uint32_t value, unsigned int n) {
	return (uint32_t)((value << n) | (value >> (-n & ((LS_BITS_DWORD) - 1))));
}

//! \brief Rotates the 64-bit \p value \p n bits to the left.
static LS_FORCE_INLINE
uint64_t __ls_rotl64(uint64_t value, unsigned int n) {
	return (uint64_t)((value << n) | (value >> (-n & ((LS_BITS_QWORD) - 1))));
}


//! \brief Rotates the 8-bit \p value \p n bits to the right.
static LS_FORCE_INLINE
uint8_t __ls_rotr8(uint8_t value, unsigned int n) {
	return (uint8_t)((value >> n) | (value << (-n & ((LS_BITS_BYTE) - 1))));
}

//! \brief Rotates the 16-bit \p value \p n bits to the right.
static LS_FORCE_INLINE
uint16_t __ls_rotr16(uint16_t value, unsigned int n) {
	return (uint16_t)((value >> n) | (value << (-n & ((LS_BITS_WORD) - 1))));
}

//! \brief Rotates the 32-bit \p value \p n bits to the right.
static LS_FORCE_INLINE
uint32_t __ls_rotr32(uint32_t value, unsigned int n) {
	return (uint32_t)((value >> n) | (value << (-n & ((LS_BITS_DWORD) - 1))));
}

//! \brief Rotates the 64-bit \p value \p n bits to the right.
static LS_FORCE_INLINE
uint64_t __ls_rotr64(uint64_t value, unsigned int n) {
	return (uint64_t)((value >> n) | (value << (-n & ((LS_BITS_QWORD) - 1))));
}




#endif
