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
**    Derived from Tarsnap/scrypt, copyright block below.
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
* This file was originally written by Colin Percival as part of the Tarsnap
* online backup system.
*/

#define FILE_PATH							"crypto/kdf/scrypt.c"

#include "./scrypt.h"
#include "../../core/memory.h"
#include "./pbkdf2-sha2.h"
#include "../symmetric/salsa20.h"


uint64_t
static integerify(const uint32_t *const B, const size_t r) {
	const uint32_t *const x2 = (uint32_t *)(((char *)B) + (((r << 1) - 1) * 64));
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
static block_salsa8_mix(const uint32_t *const Bin, uint32_t *const Bout, uint32_t *const X, const size_t r) {
	memcpy(X, &Bin[((r << 1) - 1) * 16], 64);
	
	size_t i;
	for (i = 0; i < (r << 1); i += 2) {
		block_xor(X, &Bin[i * 16], 64);
		ls_salsa20_internal_perform_rounds(X, X, 8);
		memcpy(&Bout[i * 8], X, 64);

		block_xor(X, &Bin[i * 16 + 16], 64);
		ls_salsa20_internal_perform_rounds(X, X, 8);
		memcpy(&Bout[i * 8 + r * 16], X, 64);
	}
}


ls_result_t
ls_scrypt(uint8_t *const out, const size_t out_size, const uint8_t *const LS_RESTRICT pass, const size_t pass_size, const uint8_t *const LS_RESTRICT salt, const size_t salt_size, const uint64_t N, const uint32_t r, const uint32_t p) {
	LS_RESULT_CHECK_NULL(out, 1);
	LS_RESULT_CHECK_SIZE(out_size, 1);
	LS_RESULT_CHECK_NULL(salt, 2);
	LS_RESULT_CHECK_SIZE(salt_size, 2);
	LS_RESULT_CHECK_NULL(pass, 3);
	LS_RESULT_CHECK_SIZE(pass_size, 3);
	LS_RESULT_CHECK_DATA(!r, 1);
	LS_RESULT_CHECK_DATA(!p, 2);
	LS_RESULT_CHECK_DATA(!N, 3);
	LS_RESULT_CHECK(((uint64_t)(r) * (uint64_t)(p) >= (1 << 30)), LS_RESULT_CODE_SIZE, 4);
	LS_RESULT_CHECK(((((N & (N - 1)) != 0) || (N < 2))), LS_RESULT_CODE_SIZE, 5);

	const size_t pmrm128 = (p * r * 128);
	uint8_t *B = malloc(pmrm128);

	ls_result_t pbkdf_result;
	if (!(pbkdf_result = ls_pbkdf2_sha2_256(B, pmrm128, pass, pass_size, salt, salt_size, 1)).success) {
		ls_memory_free_indirect(&B);
		return LS_RESULT_INHERITED(pbkdf_result, false);
	}

	/* Mixing scope. */ {
		const size_t rm32 = (r * 32);
		const size_t rm128 = (rm32 << 2);

		uint32_t *B32;
		uint32_t *V = malloc(rm128 * N);
		uint32_t *X = malloc((rm128 << 1) + 64);
		uint32_t *const Y = (X + rm32);
		uint32_t *const Z = (Y + rm32);

		uint64_t i;
		uint64_t j;
		uint64_t k;
		for (i = p; i--;) {
			B32 = ((uint32_t*)&B[i * 128 * r]);

#if (LS_LITTLE_ENDIAN)
			memcpy(X, B32, (sizeof(*X) * rm32));
#else
			size_t l;

			for (l = rm32; l--;) {
				X[l] = LS_SWAP_ENDIAN_LITTLE_32(B32[l]);
			}
#endif

			for (j = 0; j < N; j += 2) {
				memcpy(&V[j * rm32], X, rm128);
				block_salsa8_mix(X, Y, Z, r);

				memcpy(&V[(j + 1) * rm32], Y, rm128);
				block_salsa8_mix(Y, X, Z, r);
			}

			for (j = (N >> 1); j--;) {
				k = integerify(X, r) & (N - 1);
				block_xor(X, &V[k * rm32], rm128);
				block_salsa8_mix(X, Y, Z, r);

				k = integerify(Y, r) & (N - 1);
				block_xor(Y, &V[k * rm32], rm128);
				block_salsa8_mix(Y, X, Z, r);
			}

#if (LS_LITTLE_ENDIAN)
			memcpy(B32, X, (sizeof(*B32) * rm32));
#else
			for (l = rm32; l--;) {
				B32[l] = LS_SWAP_ENDIAN_LITTLE_32(X[l]);
			}
#endif
		}

		ls_memory_free_indirect(&V);
		ls_memory_free_indirect(&X);
	}

	pbkdf_result = ls_pbkdf2_sha2_256(out, out_size, pass, pass_size, B, pmrm128, 1);
	ls_memory_free_indirect(&B);
	if (!pbkdf_result.success) {
		return LS_RESULT_INHERITED(pbkdf_result, false);
	}

	return LS_RESULT_SUCCESS;
}
