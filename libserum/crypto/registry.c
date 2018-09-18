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


#include "./registry.h"

#include "./hashing/md5.h"
#include "./__selftests/hashing/md5.h"
#include "./hashing/ripemd160.h"
#include "./__selftests/hashing/ripemd160.h"

#include "./ciphers/salsa20.h"
#include "./__selftests/ciphers/salsa20.h"




const struct lsreg_crypto __crypto_registry[] = {
	/* Fill up 0th index. */
	{ .meta = { 0 }, .data = { { 0 } } },

	{
		.meta = {
			.selftest = lscst_hashing_md5,
			.context_size = sizeof(struct ls_md5),
			.flags = (LSREG_HASH),
			.name = "MD5",
			.maintainer = "icecubetray"
		},
		.data.hash = {
			.block_size = LS_MD5_BLOCK_SIZE,
			.digest_size = LS_MD5_DIGEST_SIZE,
			.f_init = (lssig_hash_init)ls_md5_init,
			.f_clear = NULL,
			.f_update = (lssig_hash_update)ls_md5_update,
			.f_finish = (lssig_hash_finish)ls_md5_finish
		}
	},

	{
		.meta = {
			.selftest = lscst_hashing_ripemd160,
			.context_size = sizeof(struct ls_ripemd160),
			.flags = (LSREG_HASH),
			.name = "RIPEMD-160",
			.maintainer = "icecubetray"
		},
		.data.hash = {
			.block_size = LS_RIPEMD160_BLOCK_SIZE,
			.digest_size = LS_RIPEMD160_DIGEST_SIZE,
			.f_init = (lssig_hash_init)ls_ripemd160_init,
			.f_clear = NULL,
			.f_update = (lssig_hash_update)ls_ripemd160_update,
			.f_finish = (lssig_hash_finish)ls_ripemd160_finish
		}
	},

	{
		.meta = {
			.selftest = lscst_ciphers_salsa20,
			.context_size = sizeof(union ls_salsa20),
			.flags = (LSREG_CIPHER | LS_CIPHER_STREAMABLE),
			.name = "Salsa20",
			.maintainer = "icecubetray"
		},
		.data.cipher = {
			.block_size = LS_SALSA20_BLOCK_SIZE,
			.f_init = (lssig_cipher_init)ls_salsa20_init,
			.f_clear = NULL,
			.f_rekey = (lssig_cipher_rekey)ls_salsa20_rekey,
			.f_renonce = (lssig_cipher_renonce)ls_salsa20_renonce,
			.f_get_block = (lssig_cipher_get_block)ls_salsa20_get_stream_block,
			.f_block_encrypt = (lssig_cipher_block_encrypt)ls_salsa20_block_encrypt,
			.f_block_decrypt = (lssig_cipher_block_decrypt)ls_salsa20_block_decrypt
		}
	}
};

const size_t __crypto_registry_size = sizeof(__crypto_registry);
const size_t __crypto_registry_count = (sizeof(__crypto_registry) / sizeof(*__crypto_registry));
