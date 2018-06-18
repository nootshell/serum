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


#include "../../core/setup/features.h"

#if (LSCST_ENABLED)




#include "./assert.h"

#include "../../core/memory.h"

#include <string.h>




FILEID("Assertion functions for CSTs.");




ls_result_t
lscst_hash_assert(ls_hash_algo_t algorithm, const uint8_t *const restrict data, const size_t data_size, const size_t iterations, const uint8_t *const restrict digest, uint8_t *const restrict out_digest, const size_t digest_size) {
	if (!LS_HASH_ALGORITHM_VALID(algorithm)) {
		return_e(LS_E_ALGORITHM);
	}

	if (data == NULL || digest == NULL) {
		return_e(LS_E_NULL);
	}

	if (digest_size == 0) {
		return_e(LS_E_SIZE);
	}

	if (iterations == 0) {
		return_e(LS_E_NOOP);
	}


	ls_result_t result;
	ls_hash_t hctx;


	if ((result = ls_hash_init(&hctx, algorithm)) != LS_E_SUCCESS) {
		return result;
	}


	if (data_size > 0) {
		register size_t i;
		for (i = iterations; i--;) {
			if ((result = ls_hash_update(&hctx, data, data_size)) != LS_E_SUCCESS) {
				ls_hash_clear(&hctx);
				return result;
			}
		}
	}


	if ((result = ls_hash_finish(&hctx, out_digest)) != LS_E_SUCCESS) {
		ls_hash_clear(&hctx);
		return result;
	}

	if ((result = ls_hash_clear(&hctx)) != LS_E_SUCCESS) {
		return result;
	}

	const ls_bool_t eq = (memcmp(out_digest, digest, digest_size) == 0);


	return (eq ? LS_E_SUCCESS : LS_E_DATA_MISMATCH);
}




#endif
