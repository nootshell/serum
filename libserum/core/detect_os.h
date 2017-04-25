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

#ifndef __LS_CORE_DETECT_OS_H
#define __LS_CORE_DETECT_OS_H


#define LS_OS_ID_UNKNOWN					0
#define LS_OS_ID_LINUX						1
#define LS_OS_ID_ANDROID					2
#define LS_OS_ID_MAC						3
#define LS_OS_ID_WINDOWS					0xDEFEC8ED

#if (!defined(LS_OS))
#	if (defined(__gnu_linux__) || defined(__linux__) || defined(linux))
#		define LS_LINUX						1
#		if (defined(__ANDROID__) || defined(__ANDROID_API__))
#			define LS_ANDROID				1
#			define LS_OS					LS_OS_ID_ANDROID
#			define LS_OS_STRING				"Linux (Android)"
#		else
#			define LS_OS					LS_OS_ID_LINUX
#			if (defined(DISTRO))
#				define LS_OS_STRING			"Linux (" DISTRO ")"
#			else
#				define LS_OS_STRING			"Linux"
#			endif
#		endif
#	elif (defined(_WIN32))
#		define LS_WINDOWS					1
#		define LS_OS						LS_OS_ID_WINDOWS
#		define LS_OS_STRING					"Windows"
#	elif (defined(__APPLE__) && defined(__MACH__))
#		define LS_MAC						1
#		define LS_OS						LS_OS_ID_MAC
#		define LS_OS_STRING					"Mac"
#	else
#		define LS_OS						LS_OS_ID_UNKNOWN
#		define LS_OS_STRING					"Unknown"
#	endif
#endif

#if (!defined(LS_LINUX))
#	define LS_LINUX							0
#endif

#if (!defined(LS_ANDROID))
#	define LS_ANDROID						0
#endif

#if (!defined(LS_MAC))
#	define LS_MAC							0
#endif

#if (!defined(LS_WINDOWS))
#	define LS_WINDOWS						0
#endif


#endif
