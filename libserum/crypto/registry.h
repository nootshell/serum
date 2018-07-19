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


#ifndef __LS_CRYPTO_HASH_REGISTRY_H
#define __LS_CRYPTO_HASH_REGISTRY_H




#include "../core/setup/signatures.h"

#include "./__selftests/base.h"




typedef struct lsreg_meta lsreg_meta_t;

typedef ls_result_t (*lssig_cst_case)(const lsreg_meta_t *const meta);

struct lsreg_meta {
	lssig_cst_case selftest;
	uint32_t flags;
	char name[12];
	char maintainer[32];
};




#define LS_HASH_ALGORITHM_VALID(algo)		(((algo) > 0) && ((algo < __hash_registry_count)))

#define LS_HASH_MD5							1
#define LS_HASH_RIPEMD160					2


typedef struct lsreg_hash {
	// Metadata.
	struct lsreg_meta meta;

	// Properties.
	size_t ctx_size;
	size_t block_size;
	size_t digest_size;

	// Functions.
	lssig_hash_init f_init;
	lssig_hash_clear f_clear;
	lssig_hash_update f_update;
	lssig_hash_finish f_finish;
} lsreg_hash_t;

typedef ls_nword_t ls_hash_algo_t;


extern const struct lsreg_hash __hash_registry[];

extern const size_t __hash_registry_size;
extern const size_t __hash_registry_count;




#define LS_CIPHER_ALGORITHM_VALID(algo)		(((algo) > 0) && ((algo < __cipher_registry_count)))

#define LS_CIPHER_SALSA20					1

#define LS_CIPHER_STREAMABLE				1


typedef struct lsreg_cipher {
	// Metadata.
	struct lsreg_meta meta;

	// Properties.
	size_t ctx_size;
	size_t block_size;

	// Functions.
	lssig_cipher_init f_init;
	lssig_cipher_clear f_clear;
	lssig_cipher_rekey f_rekey;
	lssig_cipher_renonce f_renonce;
	lssig_cipher_get_stream_block f_get_stream_block;
	lssig_cipher_block_encrypt f_block_encrypt;
	lssig_cipher_block_decrypt f_block_decrypt;
} lsreg_cipher_t;

typedef ls_nword_t ls_cipher_algo_t;


extern const struct lsreg_cipher __cipher_registry[];

extern const size_t __cipher_registry_size;
extern const size_t __cipher_registry_count;




#endif
