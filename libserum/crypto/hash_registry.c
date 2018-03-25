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


#include "./hash_registry.h"

#include "./hashing/base/md5.h"
#include "./selftests/hashing/base/md5.h"




FILEID("Registry of hash functions.");




#define INIT_SELFTEST(entryp)			{ .entrypoint = (entryp), .result = LS_E_NOOP, .failures = NULL, .n_failures = 0 }

struct lsreg_hashing __hash_registry[] = {
	{
		.name = "MD5",
		.ctx_size = sizeof(struct lsp_md5_data),
		.block_size = LS_MD5_BLOCK_SIZE,
		.digest_size = LS_MD5_DIGEST_SIZE,

		.init = (lssig_hash_init)lsp_md5_init,
		.clear = NULL,
		.update = (lssig_hash_update)lsp_md5_update,
		.finish = (lssig_hash_finish)lsp_md5_finish,

		.selftest = INIT_SELFTEST(lscst_hashing_md5min),
		.maintainer = "icecubetray"
	}
};

const size_t __hash_registry_size = sizeof(__hash_registry);
const size_t __hash_registry_count = (sizeof(__hash_registry) / sizeof(*__hash_registry));
