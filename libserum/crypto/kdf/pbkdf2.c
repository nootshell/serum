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

#define FILE_PATH							"crypto/kdf/pbkdf2.c"

#include "./pbkdf2.h"
#include "../../core/memory.h"
#include <string.h>


ID("PBKDF2 implementation");


ls_result_t
ls_pbkdf2(uint8_t *LS_RESTRICT out, size_t out_size, const char *const LS_RESTRICT pass, const size_t pass_size, const char *const LS_RESTRICT salt, const size_t salt_size, const uint32_t rounds, const size_t digest_size, ls_hmac_func_t const hmac) {
	LS_RESULT_CHECK_NULL(out, 1);
	LS_RESULT_CHECK_SIZE(out_size, 1);
	LS_RESULT_CHECK_NULL(pass, 2);
	LS_RESULT_CHECK_SIZE(pass_size, 2);
	LS_RESULT_CHECK_NULL(salt, 3);
	LS_RESULT_CHECK_SIZE(salt_size, 3);

	uint8_t stackalloc(d1, digest_size);
	uint8_t stackalloc(d2, digest_size);

	uint8_t stackalloc(asalt, (salt_size + sizeof(uint32_t)));
	uint32_t *asalt32 = (uint32_t*)(asalt + salt_size);
	memcpy(asalt, salt, salt_size);

	const ls_bool native_word = ((digest_size & (sizeof(unsigned int) - 1)) == 0);

	unsigned int i;
	size_t r, j;
	unsigned int count;
	for (count = 1; out_size > 0; ++count) {
		*asalt32 = LS_SWAP_ENDIAN_BIG_32(count);

		hmac(asalt, stacksizeof(asalt), pass, pass_size, d1);

		if (native_word) {
			for (i = rounds; --i;) {
				hmac(d1, digest_size, pass, pass_size, d2);

				for (j = (digest_size / sizeof(unsigned int)); j--;) {
					((unsigned int *)d1)[j] ^= ((unsigned int *)d2)[j];
				}
			}
		} else {
			for (i = rounds; --i;) {
				hmac(d1, digest_size, pass, pass_size, d2);

				for (j = digest_size; j--;) {
					d1[j] ^= d2[j];
				}
			}
		}

		r = ((out_size < digest_size) ? out_size : digest_size);
		memcpy(out, d1, r);

		out += r;
		out_size -= r;
	};

	stackfree(d1);
	stackfree(d2);
	stackfree(asalt);

	return LS_RESULT_SUCCESS;
}
