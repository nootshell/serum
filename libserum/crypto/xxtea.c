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
**    Based on implementation found at https://en.wikipedia.org/wiki/XXTEA
**
*/

#define __RFILE__							"crypto/xxtea.c"


#include "./xxtea.h"

#include <string.h>
#include <stdio.h>

#include "../core/ptrarithmetic.h"


#define XXTEA_INIT()											\
	if (!in) {													\
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);	\
	}															\
	if (!key) {													\
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 2);	\
	}															\
	if (!size) {												\
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_SIZE, 1);	\
	}															\
	if (size < 8) {												\
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_SIZE, 2);	\
	}															\
	if ((size & 3) != 0) {										\
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_SIZE, 3);	\
	}															\
																\
	size_t n = (size >> 2);										\
	size_t nm1 = (n - 1);										\
																\
	register uint32_t a, b, sum;								\
	uint_fast16_t p, e, rounds = (6 + (52 / n));				\
	a = b = sum = p = e = 0;									\
																\
	uint32_t *ptr = LS_SELECT_IO_PTR_WCPY(out, in, size);		\
																\
	const uint32_t *k = key;

#define XXTEA_ROUND							((((b >> 5) ^ (a << 2)) + ((a >> 3) ^ (b << 4))) ^ ((sum ^ a) + (k[(p & 3) ^ e] ^ b)))


ls_result_t
ls_xxtea_encrypt_ex(void *out, void *in, const size_t size, const void *key, crypto_pad_type_t padding) {
	XXTEA_INIT();

	b = ptr[nm1];
	do {
		sum += 0x9E3779B9;
		e = ((sum >> 2) & 3);
		for (p = 0; p < nm1; ++p) {
			a = ptr[p + 1];
			b = (ptr[p] += XXTEA_ROUND);
		}
		a = ptr[0];
		b = (ptr[nm1] += XXTEA_ROUND);
	} while (--rounds);

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_xxtea_encrypt(void *out, void *in, const size_t size, const void *key) {
	return ls_xxtea_encrypt_ex(out, in, size, key, PAD_NONE);
}


ls_result_t
ls_xxtea_decrypt_ex(void *out, void *in, const size_t size, const void *key, crypto_pad_type_t padding) {
	XXTEA_INIT();

	sum = (rounds * 0x9E3779B9);
	a = ptr[0];
	do {
		e = ((sum >> 2) & 3);
		for (p = n; --p > 0;) {
			b = ptr[p - 1];
			a = (ptr[p] -= XXTEA_ROUND);
		}
		b = ptr[nm1];
		a = (ptr[0] -= XXTEA_ROUND);
		sum -= 0x9E3779B9;
	} while (--rounds);

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_xxtea_decrypt(void *out, void *in, const size_t size, const void *key) {
	return ls_xxtea_decrypt_ex(out, in, size, key, PAD_NONE);
}
