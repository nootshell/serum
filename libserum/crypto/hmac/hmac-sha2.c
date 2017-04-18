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

#define FILE_PATH							"crypto/hmac/hmac-sha2.c"

#include "./hmac-sha2.h"
#include "./hmac.h"
#include <string.h>


ID("HMAC-SHA2 implementation");


ls_result_t
ls_hmac_sha2_224(const void *const LS_RESTRICT data, const size_t data_size, const void *const LS_RESTRICT key, const size_t key_size, ls_sha2_224_digest_t digest) {
	ls_sha2_224_t sha2;
	return ls_hmac_universal(
		data, data_size,
		key, key_size,
		digest, LS_SHA2_224_DIGEST_SIZE,
		LS_SHA2_224_BLOCK_SIZE, &sha2,
		(ls_hf_init_t)ls_sha2_224_init,
		(ls_hf_update_t)ls_sha2_224_update,
		(ls_hf_finish_t)ls_sha2_224_finish,
		(ls_hf_clear_t)ls_sha2_224_clear
	);
}


ls_result_t
ls_hmac_sha2_256(const void *const LS_RESTRICT data, const size_t data_size, const void *const LS_RESTRICT key, const size_t key_size, ls_sha2_256_digest_t digest) {
	ls_sha2_256_t sha2;
	return ls_hmac_universal(
		data, data_size,
		key, key_size,
		digest, LS_SHA2_256_DIGEST_SIZE,
		LS_SHA2_256_BLOCK_SIZE, &sha2,
		(ls_hf_init_t)ls_sha2_256_init,
		(ls_hf_update_t)ls_sha2_256_update,
		(ls_hf_finish_t)ls_sha2_256_finish,
		(ls_hf_clear_t)ls_sha2_256_clear
	);
}


ls_result_t
ls_hmac_sha2_384(const void *const LS_RESTRICT data, const size_t data_size, const void *const LS_RESTRICT key, const size_t key_size, ls_sha2_384_digest_t digest) {
	ls_sha2_384_t sha2;
	return ls_hmac_universal(
		data, data_size,
		key, key_size,
		digest, LS_SHA2_384_DIGEST_SIZE,
		LS_SHA2_384_BLOCK_SIZE, &sha2,
		(ls_hf_init_t)ls_sha2_384_init,
		(ls_hf_update_t)ls_sha2_384_update,
		(ls_hf_finish_t)ls_sha2_384_finish,
		(ls_hf_clear_t)ls_sha2_384_clear
	);
}


ls_result_t
ls_hmac_sha2_512(const void *const LS_RESTRICT data, const size_t data_size, const void *const LS_RESTRICT key, const size_t key_size, ls_sha2_512_digest_t digest) {
	ls_sha2_512_t sha2;
	return ls_hmac_universal(
		data, data_size,
		key, key_size,
		digest, LS_SHA2_512_DIGEST_SIZE,
		LS_SHA2_512_BLOCK_SIZE, &sha2,
		(ls_hf_init_t)ls_sha2_512_init,
		(ls_hf_update_t)ls_sha2_512_update,
		(ls_hf_finish_t)ls_sha2_512_finish,
		(ls_hf_clear_t)ls_sha2_512_clear
	);
}
