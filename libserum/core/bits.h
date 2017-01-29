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

#ifndef __LS_CORE_BITS_H
#define __LS_CORE_BITS_H


#define HAS_FLAG(flags, flag)				(((flags) & (flag)) == (flag))


#define LS_ROT(s1, s2, bits, value, shift)	(((value) s1 (shift)) | ((value) s2 (-(shift) & ((bits) - 1))))

#define LS_ROTL(bits, value, shift)			LS_ROT(<<, >>, (bits), (value), (shift))
#define LS_ROTR(bits, value, shift)			LS_ROT(>>, <<, (bits), (value), (shift))

#define LS_ROTL8(value, shift)				LS_ROTL(8, (value), (shift))
#define LS_ROTL16(value, shift)				LS_ROTL(16, (value), (shift))
#define LS_ROTL32(value, shift)				LS_ROTL(32, (value), (shift))
#define LS_ROTL64(value, shift)				LS_ROTL(64, (value), (shift))

#define LS_ROTR8(value, shift)				LS_ROTR(8, (value), (shift))
#define LS_ROTR16(value, shift)				LS_ROTR(16, (value), (shift))
#define LS_ROTR32(value, shift)				LS_ROTR(32, (value), (shift))
#define LS_ROTR64(value, shift)				LS_ROTR(64, (value), (shift))


#endif
