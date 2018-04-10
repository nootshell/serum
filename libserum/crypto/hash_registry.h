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




#include "../core/signatures.h"

#include "./selftests/base.h"




#define LS_HASH_ALGORITHM_VALID(algo)		(((algo) > 0) && ((algo < __hash_registry_count)))


#define LS_HASH_MD5							1




typedef struct lsreg_meta lsreg_meta_t;

typedef ls_result_t (*lssig_cst_case)(const lsreg_meta_t *const meta);

struct lsreg_meta {
	lssig_cst_case selftest;
	uint32_t flags;
	char name[12];
	char maintainer[32];
};

typedef struct lsreg_hash {
	// Metadata.
	struct lsreg_meta meta;

	// Properties.
	size_t ctx_size;
	size_t block_size;
	size_t digest_size;

	// Functions.
	lssig_hash_init init;
	lssig_hash_clear clear;
	lssig_hash_update update;
	lssig_hash_finish finish;
} lsreg_hash_t;


typedef ls_nword_t ls_hash_algo_t;




extern const struct lsreg_hash __hash_registry[];

extern const size_t __hash_registry_size;
extern const size_t __hash_registry_count;




#endif
