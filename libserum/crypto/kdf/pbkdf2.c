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


ID("PBKDF2 implementation");


ls_result_t
ls_pbkdf2(const char *pass, size_t pass_len, char *salt, size_t salt_len, uint8_t *key, size_t key_len, uint32_t rounds, size_t digest_size, ls_hmac_t hmac)
{
	LS_RESULT_CHECK_SIZE(rounds, 1);
	LS_RESULT_CHECK_SIZE(key_len, 2);
	LS_RESULT_CHECK_SIZE(salt_len, 3);
	LS_RESULT_CHECK_NULL(salt, 1);

	uint8_t stackalloc(obuf, digest_size);
	uint8_t stackalloc(d1, digest_size);
	uint8_t stackalloc(d2, digest_size);

	uint8_t stackalloc(asalt, (salt_len + 4));
	memcpy(asalt, salt, salt_len);

	uint32_t i, j;
	uint32_t count;
	size_t r;

	for (count = 1; key_len > 0; count++) {
		asalt[salt_len + 0] = (count >> 24) & 0xff;
		asalt[salt_len + 1] = (count >> 16) & 0xff;
		asalt[salt_len + 2] = (count >> 8) & 0xff;
		asalt[salt_len + 3] = count & 0xff;
		hmac(asalt, salt_len + 4, pass, pass_len, d1);
		memcpy(obuf, d1, digest_size);

		for (i = 1; i < rounds; i++) {
			hmac(d1, digest_size, pass, pass_len, d2);
			memcpy(d1, d2, digest_size);
			for (j = digest_size; j--;) {
				obuf[j] ^= d1[j];
			}
		}

		r = ((key_len < digest_size) ? key_len : digest_size);
		memcpy(key, obuf, r);
		key += r;
		key_len -= r;
	};

	return LS_RESULT_SUCCESS;
}


