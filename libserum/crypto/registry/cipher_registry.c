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


#include "../registry.h"

#include "../ciphers/salsa20.h"
#include "../selftests/ciphers/salsa20.h"




FILEID("Registry of ciphers.");




const struct lsreg_cipher __cipher_registry[] = {
	{ /* Fill up 0th index. */ },
	{
		.meta = {
			.selftest = lscst_ciphers_salsa20,
			.flags = LS_CIPHER_STREAMABLE,
			.name = "Salsa20",
			.maintainer = "icecubetray"
		},

		.ctx_size = sizeof(union ls_salsa20),
		.block_size = LS_SALSA20_BLOCK_SIZE,

		.f_init = (lssig_cipher_init)ls_salsa20_init,
		.f_clear = NULL,
		.f_rekey = (lssig_cipher_rekey)ls_salsa20_rekey,
		.f_renonce = (lssig_cipher_renonce)ls_salsa20_renonce,
		.f_get_stream_block = (lssig_cipher_get_stream_block)ls_salsa20_get_stream_block,
		.f_block_encrypt = (lssig_cipher_block_encrypt)ls_salsa20_block_encrypt,
		.f_block_decrypt = (lssig_cipher_block_decrypt)ls_salsa20_block_decrypt
	}
};

const size_t __cipher_registry_size = sizeof(__cipher_registry);
const size_t __cipher_registry_count = (sizeof(__cipher_registry) / sizeof(*__cipher_registry));
