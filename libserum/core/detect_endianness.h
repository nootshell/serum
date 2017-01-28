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

#ifndef __LS_CORE_DETECT_ENDIANNESS_H
#define __LS_CORE_DETECT_ENDIANNESS_H


#define LS_ENDIANNESS_ID_LITTLE				1
#define LS_ENDIANNESS_ID_BIG				2

#if (!defined(LS_ENDIANNESS))
#	if (defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIPSEB) || defined(__MIPSEB) || defined(__MIPSEB__) || ((LS_PLATFORM == LS_PLATFORM_ID_WINDOWS && REG_DWORD != REG_DWORD_LITTLE_ENDIAN) || (LS_PLATFORM != LS_PLATFORM_ID_WINDOWS && ((__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)))))
#		define LS_BIG_ENDIAN				1
#		define LS_ENDIANNESS				LS_ENDIANNESS_ID_BIG
#		define LS_ENDIANNESS_STRING			"big-endian"
#	else
#		define LS_LITTLE_ENDIAN				1
#		define LS_ENDIANNESS				LS_ENDIANNESS_ID_LITTLE
#		define LS_ENDIANNESS_STRING			"little-endian"
#	endif
#endif

#if (!defined(LS_BIG_ENDIAN))
#	define LS_BIG_ENDIAN					0
#endif

#if (!defined(LS_LITTLE_ENDIAN))
#	define LS_LITTLE_ENDIAN					0
#endif


#endif
