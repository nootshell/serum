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

#define FILE_PATH							"crypto/symmetric/rijndael.c"

#include "./rijndael.h"
#include "./rijndael-tables.h"


ID("Rijndael implementation");


// inverse mix column transform
#define RIJNDAEL_IMCT(k) (								\
	table_decrypt_0[table_encrypt_4[((k) >> 24) & 0xFF] & 0xFF] ^	\
	table_decrypt_1[table_encrypt_4[((k) >> 16) & 0xFF] & 0xFF] ^	\
	table_decrypt_2[table_encrypt_4[((k) >>  8) & 0xFF] & 0xFF] ^	\
	table_decrypt_3[table_encrypt_4[((k)      ) & 0xFF] & 0xFF]		\
)

// key expansion lookup
#define RIJNDAEL_LOOKUP_KX(z, k, rc) (					\
((z) ^ (table_encrypt_4[((k) >> 16) & 0xFF] & 0xFF000000) ^		\
	(table_encrypt_4[((k) >>  8) & 0xFF] & 0x00FF0000)	^			\
	(table_encrypt_4[((k)      ) & 0xFF] & 0x0000FF00)	^			\
	(table_encrypt_4[((k) >> 24) & 0xFF] & 0x000000FF))	^ (rc)		\
)

// key expansion lookup - 256-bits
#define RIJNDAEL_LOOKUP_KXX(z, k) (						\
	(z) ^															\
	(table_encrypt_4[((k) >> 24) & 0xFF] & 0xFF000000) ^			\
	(table_encrypt_4[((k) >> 16) & 0xFF] & 0x00FF0000) ^			\
	(table_encrypt_4[((k) >>  8) & 0xFF] & 0x0000FF00) ^			\
	(table_encrypt_4[((k)      ) & 0xFF] & 0x000000FF)				\
)

#define RIJNDAEL_ROUND_FINISH(table, v0, v1, v2, v3, k) (	\
	((table)[((v0) >> 24) & 0xFF] & 0xFF000000) ^					\
	((table)[((v1) >> 16) & 0xFF] & 0x00FF0000) ^					\
	((table)[((v2) >>  8) & 0xFF] & 0x0000FF00) ^					\
	((table)[((v3)      ) & 0xFF] & 0x000000FF) ^ (k)				\
)

#define RIJNDAEL_ROUND_ENCRYPT(v0, v1, v2, v3, k) (		\
	table_encrypt_0[((v0) >> 24) & 0xFF] ^							\
	table_encrypt_1[((v1) >> 16) & 0xFF] ^							\
	table_encrypt_2[((v2) >>  8) & 0xFF] ^							\
	table_encrypt_3[((v3)      ) & 0xFF] ^ (k)						\
)

#define RIJNDAEL_ROUND_DECRYPT(v0, v1, v2, v3, k) (		\
	table_decrypt_0[((v0) >> 24) & 0xFF] ^							\
	table_decrypt_1[((v1) >> 16) & 0xFF] ^							\
	table_decrypt_2[((v2) >>  8) & 0xFF] ^							\
	table_decrypt_3[((v3)      ) & 0xFF] ^ (k)						\
)


ls_result_t
ls_rijndael_init(ls_rijndael_t *ctx, void *key, size_t key_size) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(key, 2);
	LS_RESULT_CHECK_SIZE(key_size, 1);

#if (defined(RIJNDAEL_STRICT) && RIJNDAEL_STRICT)
	// Strict key sizes - only 16 (128-bits), 24 (192-bits), and 32 (256-bits) allowed.
	if (((key_size & ~56) != 0) || (key_size == 56)) { // TODO: benchmark
	//if (key_size != 16 && key_size != 24 && key_size != 32) {
		return LS_RESULT_ERROR_STRICT_PARAM(LS_RESULT_CODE_SIZE, 2);
	}
#else
	// No strict key sizes, accept 16 and up.
	if (key_size < 16) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_SIZE, 2);
	}
#endif

	int bits, i;
	if (key_size < 24) {
		// 128-bits
		bits = 0;
		ctx->rounds = 10;
		ctx->key_encrypt = ls_key_alloc(176);
	} else if (key_size < 32) {
		// 192-bits
		bits = 1;
		ctx->rounds = 12;
		ctx->key_encrypt = ls_key_alloc(208);
	} else {
		// 256-bits
		bits = 2;
		ctx->rounds = 14;
		ctx->key_encrypt = ls_key_alloc(240);
	}

	uint32_t
		*kin32 = key,
		*kout32 = ((uint32_t*)ctx->key_encrypt->data);

	kout32[0] = LS_SWAP_ENDIAN_BIG_32(kin32[0]);
	kout32[1] = LS_SWAP_ENDIAN_BIG_32(kin32[1]);
	kout32[2] = LS_SWAP_ENDIAN_BIG_32(kin32[2]);
	kout32[3] = LS_SWAP_ENDIAN_BIG_32(kin32[3]);

	if (bits == 0) {
		for (i = 0;;) {
			kout32[ 4] = RIJNDAEL_LOOKUP_KX(kout32[0], kout32[3], rcon[i]);
			kout32[ 5] = (kout32[ 1] ^ kout32[ 4]);
			kout32[ 6] = (kout32[ 2] ^ kout32[ 5]);
			kout32[ 7] = (kout32[ 3] ^ kout32[ 6]);

			if (++i == 10) {
				goto __expand_decrypt;
			}

			kout32 += 4;
		}
	}

	kout32[4] = LS_SWAP_ENDIAN_BIG_32(kin32[4]);
	kout32[5] = LS_SWAP_ENDIAN_BIG_32(kin32[5]);

	if (bits == 1) {
		for (i = 0;;) {
			kout32[ 6] = RIJNDAEL_LOOKUP_KX(kout32[0], kout32[5], rcon[i]);
			kout32[ 7] = (kout32[ 1] ^ kout32[ 6]);
			kout32[ 8] = (kout32[ 2] ^ kout32[ 7]);
			kout32[ 9] = (kout32[ 3] ^ kout32[ 8]);

			if (++i == 8) {
				goto __expand_decrypt;
			}

			kout32[10] = (kout32[ 4] ^ kout32[ 9]);
			kout32[11] = (kout32[ 5] ^ kout32[10]);

			kout32 += 6;
		}
	}

	kout32[6] = LS_SWAP_ENDIAN_BIG_32(kin32[6]);
	kout32[7] = LS_SWAP_ENDIAN_BIG_32(kin32[7]);

	if (bits == 2) {
		for (i = 0;;) {
			kout32[ 8] = RIJNDAEL_LOOKUP_KX(kout32[0], kout32[7], rcon[i]);
			kout32[ 9] = kout32[ 1] ^ kout32[ 8];
			kout32[10] = kout32[ 2] ^ kout32[ 9];
			kout32[11] = kout32[ 3] ^ kout32[10];

			if (++i == 7) {
				goto __expand_decrypt;
			}

			kout32[12] = RIJNDAEL_LOOKUP_KXX(kout32[4], kout32[11]);
			kout32[13] = kout32[ 5] ^ kout32[12];
			kout32[14] = kout32[ 6] ^ kout32[13];
			kout32[15] = kout32[ 7] ^ kout32[14];

			kout32 += 8;
		}
	}

__expand_decrypt:
	ctx->key_decrypt = ls_key_clone(ctx->key_encrypt);
	kout32 = ((uint32_t*)ctx->key_decrypt->data);

	uint32_t t, j;
	for (i = 0, j = (ctx->rounds << 2); i < j; i += 4, j -= 4) {
		t             = kout32[i    ];
		kout32[i    ] = kout32[j    ];
		kout32[j    ] = t            ;

		t             = kout32[i + 1];
		kout32[i + 1] = kout32[j + 1];
		kout32[j + 1] = t            ;

		t             = kout32[i + 2];
		kout32[i + 2] = kout32[j + 2];
		kout32[j + 2] = t            ;

		t             = kout32[i + 3];
		kout32[i + 3] = kout32[j + 3];
		kout32[j + 3] = t            ;
	}

	for (i = (ctx->rounds - 1); i--;) {
		kout32 += 4;
		kout32[0] = RIJNDAEL_IMCT(kout32[0]);
		kout32[1] = RIJNDAEL_IMCT(kout32[1]);
		kout32[2] = RIJNDAEL_IMCT(kout32[2]);
		kout32[3] = RIJNDAEL_IMCT(kout32[3]);
	}
}
