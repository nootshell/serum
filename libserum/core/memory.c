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


#include "./memory.h"

#include <inttypes.h>




static const char fmt_normal[] = "%0*"PRIXPTR": ";
static const char fmt_ansi[] = "\033[0;36m%0*"PRIXPTR"\033[0m: \033[0;37m";




ls_result_t
ls_memory_dump_ex(const void *const src, const size_t size, const ls_nword_t columns, const ls_nword_t columnsize, FILE *const dest, const ls_bool_t ansi) {
	if (dest == NULL) {
		return LS_E_NULL;
	}

	if (columns < 1 || columnsize < 1) {
		return LS_E_INVALID;
	}

	if (size < 1) {
		return LS_E_NOOP;
	}


	const ls_uint8_t *src8 = src;
	const char *const pfmt = (ansi ? fmt_ansi : fmt_normal);
	const size_t pfmt_sz = ((sizeof(void*) << 1) + (ansi ? 18 : 0) + 2);

	ls_result_t result = LS_E_SUCCESS;


	size_t i, word, column;
	for (i = size, word = column = 0; i--;) {
		if (column == 0) {
			if (fprintf(dest, pfmt, (int)(sizeof(void*) << 1), (uintptr_t)src8) != pfmt_sz) {
				result = LS_E_IO_WRITE;
				break;
			}
		}

		if (fprintf(dest, "%02X", *src8++) != 2) {
			// We expect to write 2 characters, and we're strict about this.
			result = LS_E_IO_WRITE;
			break;
		}

		if (++word == columnsize) {
			word = 0;

			if (++column == columns) {
				column = 0;

				if (fputs(LS_EOL, dest) == EOF) {
					result = LS_E_IO_WRITE;
					break;
				}
			} else {
				if (fputs(" ", dest) == EOF) {
					result = LS_E_IO_WRITE;
					break;
				}
			}
		}
	}


	if (ansi && fputs("\033[0m", dest) == EOF) {
		return LS_E_IO_WRITE;
	}

	if (word != 0 || column != 0) {
		if (fputs(LS_EOL, dest) == EOF) {
			return LS_E_IO_WRITE;
		}
	}

	if (fflush(dest) != 0) {
		return LS_E_IO_FLUSH;
	}


	return result;
}
