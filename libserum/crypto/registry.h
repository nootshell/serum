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


typedef unsigned int ls_crypto_algo_t;




#define LSREG_HASH							0x0001
#define LSREG_CIPHER						0x0002
#define LS_CIPHER_STREAMABLE				0x0100 //!< \hideinitializer \brief Meta flag indicating that the cipher is streamable.


//! \hideinitializer \brief Checks if the given algorithm is a valid, registered algorithm and has the given flags set.
#define LSREG_CRYPTO_VALID(algo, _flags)	((((algo) > 0) && ((algo) < __crypto_registry_count)) && LS_FLAG(__crypto_registry[(algo)].meta.flags, (_flags)))

//! \hideinitializer \brief
#define LSREG_CRYPTO_HASH_VALID(algo)		LSREG_CRYPTO_VALID((algo), LSREG_HASH)

//! \hideinitializer \brief
#define LSREG_CRYPTO_CIPHER_VALID(algo)		LSREG_CRYPTO_VALID((algo), LSREG_CIPHER)




#define LS_HASH_MD5							1	//!< \hideinitializer \brief The MD5 algorithm.
#define LS_HASH_RIPEMD160					2	//!< \hideinitializer \brief The RIPEMD160 algorithm.

#define LS_CIPHER_SALSA20					3	//!< \hideinitializer \brief The Salsa20 algorithm.




extern const struct lsreg_crypto {
	struct lsreg_meta {
		lssig_cst_case selftest;
		size_t context_size;
		uint32_t flags;
		char name[12];
		char maintainer[32];
	} meta;
	union {
		struct lsreg_crypto_hash {
			size_t block_size;
			size_t digest_size;
			lssig_hash_init f_init;
			lssig_hash_clear f_clear;
			lssig_hash_update f_update;
			lssig_hash_finish f_finish;
		} hash;
		struct lsreg_crypto_cipher {
			size_t block_size;
			lssig_cipher_init f_init;
			lssig_cipher_clear f_clear;
			lssig_cipher_rekey f_rekey;
			lssig_cipher_renonce f_renonce;
			lssig_cipher_get_block f_get_block;
			lssig_cipher_block_encrypt f_block_encrypt;
			lssig_cipher_block_decrypt f_block_decrypt;
		} cipher;
	} data;
} __crypto_registry[];

extern const size_t __crypto_registry_size;
extern const size_t __crypto_registry_count;




#endif
