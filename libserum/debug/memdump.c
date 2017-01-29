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

#define __RFILE__							"core/memdump.c"

#include "./memdump.h"

#include <stdio.h>


void
ls_memdump_ex(const void *ptr, const size_t size, int columns, int items_per_column) {
	if (!ptr || !size) {
		return;
	}

	if (columns < 1) {
		columns = 1;
	}
	if (items_per_column < 1) {
		items_per_column = 1;
	}

	int c = 0, ic = 0;
    size_t i;
    if (items_per_column == 1) {
		for (i = 0; i < size; ++i) {
			printf("%02X ", ((uint8_t*)ptr)[i]);
			if (++c >= columns) {
				c = 0;
				puts("");
			}
		}
    } else {
		for (i = 0; i < size; ++i) {
            printf("%02X", ((uint8_t*)ptr)[i]);
			if (++ic >= items_per_column) {
				ic = 0;
				if (++c >= columns) {
					c = 0;
					puts("");
				} else {
					fputs(" ", stdout);
				}
			}
		}
    }
    if (c > 0) {
		puts("");
    }
}


void
ls_memdump(const void *ptr, const size_t size) {
    ls_memdump_ex(ptr, size, 16, 1);
}


void
ls_vmemdump_ex(const void *ptr, const size_t size, int columns, int items_per_column, const char *str) {
	if (!ptr || !size) {
		return;
	}

	if (str) {
		puts(str);
	}

	ls_memdump_ex(ptr, size, columns, items_per_column);
}


void
ls_vmemdump(const void *ptr, const size_t size, const char *str) {
	ls_vmemdump_ex(ptr, size, 16, 1, str);
}
