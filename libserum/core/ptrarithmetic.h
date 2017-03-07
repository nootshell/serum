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

#ifndef __LS_CORE_PTRARITHMETIC_H
#define __LS_CORE_PTRARITHMETIC_H


#define _LS_PTR_DIFF(ptr1, ptr2)			(((char*)(ptr1)) - ((char*)(ptr2)))
#define LS_PTR_DIFF(ptr1, ptr2)				((size_t)((((char*)(ptr1)) > ((char*)(ptr2))) ? _LS_PTR_DIFF((ptr1), (ptr2)) : _LS_PTR_DIFF((ptr2), (ptr1))))

#define LS_SELECT_IO_PTR(out, in)			((out) ? (out) : (in))

#define LS_SELECT_IO_PTR_WCPY(out, in, sz)	(((out)) ? (((out) != (in)) ? memcpy((out), (in), sz) : (out)) : (in))
/*
**  The code above translates to:
**	if ((out)) {
**		if ((out) != (in)) {
**			return _mcpy((out), (in), (sz));
**		} else {
**			return (out);
**		}
**	} else {
**		return (in);
**	}
**
*/


#endif
