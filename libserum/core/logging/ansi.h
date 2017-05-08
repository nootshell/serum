/*******************************************************************************
 * *                                                                            **
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

#ifndef __LS_CORE_LOGGING_ANSI_H
#define __LS_CORE_LOGGING_ANSI_H


#include "../defaults.h"
#include "../detect_os.h"
#include "../detect_compiler.h"


// Little less messy, bit more verbose.
#if (LS_ANSI_ESCAPE_SUPPORT)
#	define __LS_ANSI_PUT(x)					x
#else
#	define __LS_ANSI_PUT(x)
#endif


// Font
#define LS_ANSI_SET_FONT(n)					__LS_ANSI_PUT("1" MACRO_STRINGIFY(n))
#define LS_ANSI_SET_FONT_PRIMARY			LS_ANSI_SET_FONT("0")

// Font weight, intensity
#define _LS_ANSI_SGR_BOLDINTENSE			__LS_ANSI_PUT("1")
#define _LS_ANSI_SGR_LESSINTENSE			__LS_ANSI_PUT("2")
#define _LS_ANSI_SGR_BOLD_OFF_UNDERLINE_DBL __LS_ANSI_PUT("21")

// Styling
#define LS_ANSI_OPT_ITALIC					__LS_ANSI_PUT("3")
#define LS_ANSI_OPT_FRAKTUR					__LS_ANSI_PUT("20")
#define LS_ANSI_OPT_NO_ITALIC_NO_FRAKTUR	__LS_ANSI_PUT("23")

// Blinking
#define LS_ANSI_OPT_BLINK_SLOW				__LS_ANSI_PUT("5")
#define LS_ANSI_OPT_BLINK_RAPID				__LS_ANSI_PUT("6")
#define LS_ANSI_OPT_BLINK_OFF				__LS_ANSI_PUT("25")

// Lines
#define LS_ANSI_OPT_CROSS_OUT				__LS_ANSI_PUT("9")
#define LS_ANSI_OPT_NO_CROSS_OUT			__LS_ANSI_PUT("29")
#define LS_ANSI_OPT_UNDERLINE_SINGLE		__LS_ANSI_PUT("4")
#define LS_ANSI_OPT_NO_UNDERLINE			__LS_ANSI_PUT("24")
#define LS_ANSI_OPT_FRAME					__LS_ANSI_PUT("51")
#define LS_ANSI_OPT_ENCIRCLE				__LS_ANSI_PUT("52")
#define LS_ANSI_OPT_OVERLINE				__LS_ANSI_PUT("53")
#define LS_ANSI_OPT_NO_FRAME_NO_ENCIRCLE	__LS_ANSI_PUT("54")
#define LS_ANSI_OPT_NO_OVERLINE				__LS_ANSI_PUT("55")

// Misc.
#define _LS_ANSI_SGR_RESET					__LS_ANSI_PUT("0")
#define LS_ANSI_OPT_NORMAL					__LS_ANSI_PUT("22")
#define LS_ANSI_OPT_NEGATIVE				__LS_ANSI_PUT("7")
#define LS_ANSI_OPT_POSITIVE				__LS_ANSI_PUT("27")
#define LS_ANSI_OPT_CONCEAL					__LS_ANSI_PUT("8")
#define LS_ANSI_OPT_REVEAL					__LS_ANSI_PUT("28")


// Color Ranges
#define _LS_ANSI_SGR_CRF_NORMAL				__LS_ANSI_PUT("3")
#define _LS_ANSI_SGR_CRB_NORMAL				__LS_ANSI_PUT("4")
#define _LS_ANSI_SGR_CRF_BRIGHT				__LS_ANSI_PUT("9")
#define _LS_ANSI_SGR_CRB_BRIGHT				__LS_ANSI_PUT("10")

// Colors
#define _LS_ANSI_SGR_C_BLACK				__LS_ANSI_PUT("0")
#define _LS_ANSI_SGR_C_RED					__LS_ANSI_PUT("1")
#define _LS_ANSI_SGR_C_GREEN				__LS_ANSI_PUT("2")
#define _LS_ANSI_SGR_C_YELLOW				__LS_ANSI_PUT("3")
#define _LS_ANSI_SGR_C_BLUE					__LS_ANSI_PUT("4")
#define _LS_ANSI_SGR_C_MAGENTA				__LS_ANSI_PUT("5")
#define _LS_ANSI_SGR_C_CYAN					__LS_ANSI_PUT("6")
#define _LS_ANSI_SGR_C_WHITE				__LS_ANSI_PUT("7")
#define _LS_ANSI_SGR_C_DEFAULT				__LS_ANSI_PUT("9")

#define _LS_ANSI_SGR_MKCOLOR(r, c)			__LS_ANSI_PUT(r c)
#define _LS_ANSI_SGR_MKFCOLOR_NORMAL(c)		_LS_ANSI_SGR_MKCOLOR(_LS_ANSI_SGR_CRF_NORMAL, c)
#define _LS_ANSI_SGR_MKBCOLOR_NORMAL(c)		_LS_ANSI_SGR_MKCOLOR(_LS_ANSI_SGR_CRB_NORMAL, c)
#define _LS_ANSI_SGR_MKFCOLOR_BRIGHT(c)		_LS_ANSI_SGR_MKCOLOR(_LS_ANSI_SGR_CRF_BRIGHT, c)
#define _LS_ANSI_SGR_MKBCOLOR_BRIGHT(c)		_LS_ANSI_SGR_MKCOLOR(_LS_ANSI_SGR_CRB_BRIGHT, c)
#define _LS_ANSI_SGR_MKFCOLOR_DIMMED(c)		_LS_ANSI_SGR_MKFCOLOR_NORMAL(c) LS_ANSI_OPT _LS_ANSI_SGR_LESSINTENSE
#define _LS_ANSI_SGR_MKBCOLOR_DIMMED(c)		_LS_ANSI_SGR_MKBCOLOR_NORMAL(c) LS_ANSI_OPT _LS_ANSI_SGR_LESSINTENSE

#define LS_ANSI_FG_BLACK					_LS_ANSI_SGR_MKFCOLOR_NORMAL(_LS_ANSI_SGR_C_BLACK	)
#define LS_ANSI_BG_BLACK					_LS_ANSI_SGR_MKBCOLOR_NORMAL(_LS_ANSI_SGR_C_BLACK	)
#define LS_ANSI_FG_DARK_GRAY				_LS_ANSI_SGR_MKFCOLOR_BRIGHT(_LS_ANSI_SGR_C_BLACK	)
#define LS_ANSI_BG_DARK_GRAY				_LS_ANSI_SGR_MKBCOLOR_BRIGHT(_LS_ANSI_SGR_C_BLACK	)
#define LS_ANSI_FG_RED						_LS_ANSI_SGR_MKFCOLOR_NORMAL(_LS_ANSI_SGR_C_RED		)
#define LS_ANSI_BG_RED						_LS_ANSI_SGR_MKBCOLOR_NORMAL(_LS_ANSI_SGR_C_RED		)
#define LS_ANSI_FG_BRIGHT_RED				_LS_ANSI_SGR_MKFCOLOR_BRIGHT(_LS_ANSI_SGR_C_RED		)
#define LS_ANSI_BG_BRIGHT_RED				_LS_ANSI_SGR_MKBCOLOR_BRIGHT(_LS_ANSI_SGR_C_RED		)
#define LS_ANSI_FG_GREEN					_LS_ANSI_SGR_MKFCOLOR_NORMAL(_LS_ANSI_SGR_C_GREEN	)
#define LS_ANSI_BG_GREEN					_LS_ANSI_SGR_MKBCOLOR_NORMAL(_LS_ANSI_SGR_C_GREEN	)
#define LS_ANSI_FG_LIME						_LS_ANSI_SGR_MKFCOLOR_BRIGHT(_LS_ANSI_SGR_C_GREEN	)
#define LS_ANSI_BG_LIME						_LS_ANSI_SGR_MKBCOLOR_BRIGHT(_LS_ANSI_SGR_C_GREEN	)
#define LS_ANSI_FG_ORANGE					_LS_ANSI_SGR_MKFCOLOR_NORMAL(_LS_ANSI_SGR_C_YELLOW	)
#define LS_ANSI_BG_ORANGE					_LS_ANSI_SGR_MKBCOLOR_NORMAL(_LS_ANSI_SGR_C_YELLOW	)
#define LS_ANSI_FG_YELLOW					_LS_ANSI_SGR_MKFCOLOR_BRIGHT(_LS_ANSI_SGR_C_YELLOW	)
#define LS_ANSI_BG_YELLOW					_LS_ANSI_SGR_MKBCOLOR_BRIGHT(_LS_ANSI_SGR_C_YELLOW	)
#define LS_ANSI_FG_BLUE						_LS_ANSI_SGR_MKFCOLOR_NORMAL(_LS_ANSI_SGR_C_BLUE	)
#define LS_ANSI_BG_BLUE						_LS_ANSI_SGR_MKBCOLOR_NORMAL(_LS_ANSI_SGR_C_BLUE	)
#define LS_ANSI_FG_BRIGHT_BLUE				_LS_ANSI_SGR_MKFCOLOR_BRIGHT(_LS_ANSI_SGR_C_BLUE	)
#define LS_ANSI_BG_BRIGHT_BLUE				_LS_ANSI_SGR_MKBCOLOR_BRIGHT(_LS_ANSI_SGR_C_BLUE	)
#define LS_ANSI_FG_MAGENTA					_LS_ANSI_SGR_MKFCOLOR_NORMAL(_LS_ANSI_SGR_C_MAGENTA	)
#define LS_ANSI_BG_MAGENTA					_LS_ANSI_SGR_MKBCOLOR_NORMAL(_LS_ANSI_SGR_C_MAGENTA	)
#define LS_ANSI_FG_PINK						_LS_ANSI_SGR_MKFCOLOR_BRIGHT(_LS_ANSI_SGR_C_MAGENTA	)
#define LS_ANSI_BG_PINK						_LS_ANSI_SGR_MKBCOLOR_BRIGHT(_LS_ANSI_SGR_C_MAGENTA	)
#define LS_ANSI_FG_CYAN						_LS_ANSI_SGR_MKFCOLOR_NORMAL(_LS_ANSI_SGR_C_CYAN	)
#define LS_ANSI_BG_CYAN						_LS_ANSI_SGR_MKBCOLOR_NORMAL(_LS_ANSI_SGR_C_CYAN	)
#define LS_ANSI_FG_CYAN2					_LS_ANSI_SGR_MKFCOLOR_BRIGHT(_LS_ANSI_SGR_C_CYAN	)
#define LS_ANSI_BG_CYAN2					_LS_ANSI_SGR_MKBCOLOR_BRIGHT(_LS_ANSI_SGR_C_CYAN	)
#define LS_ANSI_FG_GRAY						_LS_ANSI_SGR_MKFCOLOR_DIMMED(_LS_ANSI_SGR_C_WHITE	)
#define LS_ANSI_BG_GRAY						_LS_ANSI_SGR_MKBCOLOR_DIMMED(_LS_ANSI_SGR_C_WHITE	)
#define LS_ANSI_FG_LIGHT_GRAY				_LS_ANSI_SGR_MKFCOLOR_NORMAL(_LS_ANSI_SGR_C_WHITE	)
#define LS_ANSI_BG_LIGHT_GRAY				_LS_ANSI_SGR_MKBCOLOR_NORMAL(_LS_ANSI_SGR_C_WHITE	)
#define LS_ANSI_FG_WHITE					_LS_ANSI_SGR_MKFCOLOR_BRIGHT(_LS_ANSI_SGR_C_WHITE	)
#define LS_ANSI_BG_WHITE					_LS_ANSI_SGR_MKBCOLOR_BRIGHT(_LS_ANSI_SGR_C_WHITE	)


#define LS_ANSI_BEGIN						__LS_ANSI_PUT("\x1B[")
#define LS_ANSI_OPT							__LS_ANSI_PUT(";")
#define LS_ANSI_END							__LS_ANSI_PUT("m")

#define LS_ANSI_1(o)						LS_ANSI_BEGIN o LS_ANSI_END
#define LS_ANSI_2(o1, o2)					LS_ANSI_1(o1 LS_ANSI_OPT o2)
#define LS_ANSI_3(o1, o2, o3)				LS_ANSI_1(o1 LS_ANSI_OPT o2 LS_ANSI_OPT o3)
#define LS_ANSI_4(o1, o2, o3, o4)			LS_ANSI_1(o1 LS_ANSI_OPT o2 LS_ANSI_OPT o3 LS_ANSI_OPT o4)
#define LS_ANSI_5(o1, o2, o3, o4, o5)		LS_ANSI_1(o1 LS_ANSI_OPT o2 LS_ANSI_OPT o3 LS_ANSI_OPT o4 LS_ANSI_OPT o5)
#define LS_ANSI_6(o1, o2, o3, o4, o5, o6)	LS_ANSI_1(o1 LS_ANSI_OPT o2 LS_ANSI_OPT o3 LS_ANSI_OPT o4 LS_ANSI_OPT o5 LS_ANSI_OPT o6)

#define LS_ANSI_RESET						LS_ANSI_1(_LS_ANSI_SGR_RESET)


#endif
