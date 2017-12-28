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
**    Derived from Tarsnap/scrypt, copyright block below. Heavily modified.
**
*/

/*-
* Copyright 2009 Colin Percival
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
*/

#define FILE_PATH							"crypto/kdf/scrypt.c"

#include "./scrypt.h"
#include "./_signatures.h"
#include "./scrypt-sha2.h"
#include "../symmetric/salsa20.h"
#include "../../core/memory.h"
#include <string.h>


ID("pluggable scrypt implementation");


uint64_t
static integerify(const uint32_t *const B, const size_t r) {
	const uint32_t *const x2 = (uint32_t *)(((char *)B) + (((r << 1) - 1) << 6));
	return ((((uint64_t)x2[1]) << 32) | x2[0]);
}


void
static block_xor(ls_nword_t *const dest, const ls_nword_t *const src, const size_t size) {
	size_t n = (size / sizeof(*dest));
	for (; n--;) {
		dest[n] ^= src[n];
	}
}


void
static block_mix_salsa8(const uint32_t *const in, uint32_t *const out, uint32_t *const X, const size_t rshl1, const size_t rshl4) {
	memcpy(X, &in[(rshl1 - 1) << 4], 64);

	size_t i;
	for (i = 0; i < rshl1; i += 2) {
		block_xor(X, &in[i << 4], 64);
		ls_salsa20_internal_perform_rounds(X, X, 8);
		memcpy(&out[(i << 3)], X, 64);

		block_xor(X, &in[(i << 4) + 16], 64);
		ls_salsa20_internal_perform_rounds(X, X, 8);
		memcpy(&out[(i << 3) + rshl4], X, 64);
	}
}


ls_result_t
ls_scrypt_universal(uint8_t *const out, const size_t out_size, const char *const LS_RESTRICT pass, const size_t pass_size, const char *const LS_RESTRICT salt, const size_t salt_size, const ls_nword_t inner_rounds, const ls_nword_t weight, const ls_nword_t outer_rounds, ls_kdf_func_t kdf) {
	LS_RESULT_CHECK_NULL(out, 1);
	LS_RESULT_CHECK_NULL(pass, 2);
	LS_RESULT_CHECK_NULL(salt, 3);
	LS_RESULT_CHECK_NULL(kdf, 4);

	LS_RESULT_CHECK_SIZE(out_size, 1);
#if (LS_KDF_DENY_PASS_SIZE_ZERO)
	LS_RESULT_CHECK_SIZE(pass_size, 2);
#endif
#if (LS_KDF_DENY_SALT_SIZE_ZERO)
	LS_RESULT_CHECK_SIZE(salt_size, 3);
#endif


	LS_RESULT_CHECK_DATA(!weight, 1);
	LS_RESULT_CHECK_DATA(!outer_rounds, 2);
	LS_RESULT_CHECK_DATA(!inner_rounds, 3);

	LS_RESULT_CHECK(((((uint64_t)weight) * ((uint64_t)outer_rounds)) >= 0x40000000LLU), LS_RESULT_CODE_SIZE, 4);
	LS_RESULT_CHECK(((((inner_rounds & (inner_rounds - 1)) != 0) || (inner_rounds < 2))), LS_RESULT_CODE_SIZE, 5);


	const size_t pmrshl7 = (outer_rounds * weight << 7);
	//uint8_t *B = malloc(pmrshl7);

	ls_memory_area_t B_area;
	if (!ls_memory_area_init(&B_area, pmrshl7).success) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_MISC); // TODO
	}
	uint8_t *const B = B_area.data;

	ls_result_t pbkdf_result;
	if (!(pbkdf_result = kdf(B, pmrshl7, pass, pass_size, salt, salt_size, 1)).success) {
		ls_memory_area_clear(&B_area);
		return LS_RESULT_INHERITED(pbkdf_result, false);
	}

	/* Mixing scope. */ {
		const size_t wshl1 = (weight << 1);
		const size_t wshl4 = (weight << 4);
		const size_t wshl5 = (weight << 5);
		const size_t wshl7 = (weight << 7);
		const uint64_t irshr1 = (inner_rounds >> 1);
		const uint64_t irs1 = (inner_rounds - 1);


		ls_memory_area_t V_area;
		if (!ls_memory_area_init(&V_area, (wshl7 * inner_rounds)).success) {
			return LS_RESULT_ERROR(LS_RESULT_CODE_MISC); // TODO
		}
		uint32_t *const V = V_area.data;

		ls_memory_area_t X_area;
		if (!ls_memory_area_init(&X_area, ((wshl7 << 1) + 64)).success) {
			return LS_RESULT_ERROR(LS_RESULT_CODE_MISC); // TODO
		}
		uint32_t *const X = X_area.data;


		uint32_t *const Y = (X + wshl5);
		uint32_t *const Z = (Y + wshl5);
		uint32_t *B32;

		uint64_t j;
		uint64_t k;

		uint64_t i;
		for (i = outer_rounds; i--;) {
			B32 = ((uint32_t *const)&B[wshl7 * i]);

#if (LS_LITTLE_ENDIAN)
			memcpy(X, B32, (sizeof(*X) * wshl5));
#else
			size_t l;

			for (l = wshl5; l--;) {
				X[l] = LS_SWAP_ENDIAN_LITTLE_32(B32[l]);
			}
#endif

			for (j = 0; j < inner_rounds; j += 2) {
				memcpy(&V[j * wshl5], X, wshl7);
				block_mix_salsa8(X, Y, Z, wshl1, wshl4);

				memcpy(&V[(j + 1) * wshl5], Y, wshl7);
				block_mix_salsa8(Y, X, Z, wshl1, wshl4);
			}

			for (j = irshr1; j--;) {
				k = (integerify(X, weight) & irs1);
				block_xor(X, &V[k * wshl5], wshl7);
				block_mix_salsa8(X, Y, Z, wshl1, wshl4);

				k = (integerify(Y, weight) & irs1);
				block_xor(Y, &V[k * wshl5], wshl7);
				block_mix_salsa8(Y, X, Z, wshl1, wshl4);
			}

#if (LS_LITTLE_ENDIAN)
			memcpy(B32, X, (sizeof(*B32) * wshl5));
#else
			for (l = wshl5; l--;) {
				B32[l] = LS_SWAP_ENDIAN_LITTLE_32(X[l]);
			}
#endif
		}

		ls_memory_area_clear(&V_area);
		ls_memory_area_clear(&X_area);
	}

	pbkdf_result = kdf(out, out_size, pass, pass_size, B, pmrshl7, 1);
	ls_memory_area_clear(&B_area);
	if (!pbkdf_result.success) {
		return LS_RESULT_INHERITED(pbkdf_result, false);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_scrypt(uint8_t *const out, const size_t out_size, const char *const LS_RESTRICT pass, const size_t pass_size, const char *const LS_RESTRICT salt, const size_t salt_size, const ls_nword_t inner_rounds, const ls_nword_t weight, const ls_nword_t outer_rounds) {
	return ls_scrypt_sha2_256(
		out, out_size,
		pass, pass_size,
		salt, salt_size,
		inner_rounds, weight, outer_rounds
	);
}
