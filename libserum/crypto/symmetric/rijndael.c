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
#include "./__rijndael-tables.h"


ID("Rijndael implementation");


// inverse mix column transform
#define RIJNDAEL_IMCT(k) (											\
	table_decrypt_0[table_encrypt_4[((k) >> 24) & 0xFF] & 0xFF] ^	\
	table_decrypt_1[table_encrypt_4[((k) >> 16) & 0xFF] & 0xFF] ^	\
	table_decrypt_2[table_encrypt_4[((k) >>  8) & 0xFF] & 0xFF] ^	\
	table_decrypt_3[table_encrypt_4[((k)      ) & 0xFF] & 0xFF]		\
)

// key expansion lookup
#define RIJNDAEL_LOOKUP_KX(z, k, rc) (								\
	((z) ^															\
	(table_encrypt_4[((k) >> 16) & 0xFF] & 0xFF000000)  ^			\
	(table_encrypt_4[((k) >>  8) & 0xFF] & 0x00FF0000)	^			\
	(table_encrypt_4[((k)      ) & 0xFF] & 0x0000FF00)	^			\
	(table_encrypt_4[((k) >> 24) & 0xFF] & 0x000000FF))	^ (rc)		\
)

// key expansion lookup - 256-bits
#define RIJNDAEL_LOOKUP_KXX(z, k) (									\
	(z) ^															\
	(table_encrypt_4[((k) >> 24) & 0xFF] & 0xFF000000) ^			\
	(table_encrypt_4[((k) >> 16) & 0xFF] & 0x00FF0000) ^			\
	(table_encrypt_4[((k) >>  8) & 0xFF] & 0x0000FF00) ^			\
	(table_encrypt_4[((k)      ) & 0xFF] & 0x000000FF)				\
)

#define RIJNDAEL_ROUND_FINISH(table, v0, v1, v2, v3, k) (			\
	((table)[((v0) >> 24) & 0xFF] & 0xFF000000) ^					\
	((table)[((v1) >> 16) & 0xFF] & 0x00FF0000) ^					\
	((table)[((v2) >>  8) & 0xFF] & 0x0000FF00) ^					\
	((table)[((v3)      ) & 0xFF] & 0x000000FF) ^ (k)				\
)

#define RIJNDAEL_ROUND_ENCRYPT(v0, v1, v2, v3, k) (					\
	table_encrypt_0[((v0) >> 24) & 0xFF] ^							\
	table_encrypt_1[((v1) >> 16) & 0xFF] ^							\
	table_encrypt_2[((v2) >>  8) & 0xFF] ^							\
	table_encrypt_3[((v3)      ) & 0xFF] ^ (k)						\
)

#define RIJNDAEL_ROUND_DECRYPT(v0, v1, v2, v3, k) (					\
	table_decrypt_0[((v0) >> 24) & 0xFF] ^							\
	table_decrypt_1[((v1) >> 16) & 0xFF] ^							\
	table_decrypt_2[((v2) >>  8) & 0xFF] ^							\
	table_decrypt_3[((v3)      ) & 0xFF] ^ (k)						\
)


ls_result_t
ls_rijndael_init(ls_rijndael_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT key, const size_t key_size) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(key, 2);
	LS_RESULT_CHECK_SIZE(key_size, 1);

#if (LS_RIJNDAEL_STRICT)
	// Strict key sizes - only 16 (128-bits), 24 (192-bits), and 32 (256-bits) allowed.

	//if (((key_size & ~56) != 0) || (key_size == 56)) {
	if ((key_size != 16) && (key_size != 24) && (key_size != 32)) {
		return LS_RESULT_ERROR_STRICT_PARAM(LS_RESULT_CODE_SIZE, 2);
	}
#else
	// No strict key sizes, accept 16 and up.

	if (key_size < 16) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_SIZE, 2);
	}
#endif

	if (key_size < 24) {
		// 128-bits
		ctx->flags = LS_RIJNDAEL_128;
		ctx->rounds = 10;
		ctx->key_encrypt = ls_key_alloc(176);
	} else if (key_size < 32) {
		// 192-bits
		ctx->flags = LS_RIJNDAEL_192;
		ctx->rounds = 12;
		ctx->key_encrypt = ls_key_alloc(208);
	} else {
		// 256-bits
		ctx->flags = LS_RIJNDAEL_256;
		ctx->rounds = 14;
		ctx->key_encrypt = ls_key_alloc(240);
	}

	ls_nword_t i;
	const uint32_t *kin32 = key;
	uint32_t *kout32 = ((uint32_t*)ctx->key_encrypt->data);

	kout32[0] = LS_SWAP_ENDIAN_BIG_32(kin32[0]);
	kout32[1] = LS_SWAP_ENDIAN_BIG_32(kin32[1]);
	kout32[2] = LS_SWAP_ENDIAN_BIG_32(kin32[2]);
	kout32[3] = LS_SWAP_ENDIAN_BIG_32(kin32[3]);

	if (HAS_FLAG(ctx->flags, LS_RIJNDAEL_128)) {
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

	if (HAS_FLAG(ctx->flags, LS_RIJNDAEL_192)) {
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

	if (HAS_FLAG(ctx->flags, LS_RIJNDAEL_256)) {
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

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_rijndael_clear(ls_rijndael_t *const ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	if (ctx->key_encrypt) {
		ctx->key_encrypt = ls_key_free(ctx->key_encrypt);
	}

	if (ctx->key_decrypt) {
		ctx->key_decrypt = ls_key_free(ctx->key_decrypt);
	}

	ctx->flags = 0;
	ctx->rounds = 0;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_rijndael_encrypt_block(const ls_rijndael_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(block, 2);
	LS_RESULT_CHECK_NULL(ctx->key_encrypt, 3);

	const uint32_t *key = ((uint32_t*)ctx->key_encrypt->data);

	register uint32_t
		s0, s1, s2, s3,
		t0, t1, t2, t3;

	s0 = (LS_SWAP_ENDIAN_BIG_32(block[0]) ^ key[0]);
	s1 = (LS_SWAP_ENDIAN_BIG_32(block[1]) ^ key[1]);
	s2 = (LS_SWAP_ENDIAN_BIG_32(block[2]) ^ key[2]);
	s3 = (LS_SWAP_ENDIAN_BIG_32(block[3]) ^ key[3]);

	ls_nword_t r = (ctx->rounds >> 1);
	for (;;) {
		t0 = RIJNDAEL_ROUND_ENCRYPT(s0, s1, s2, s3, key[4]);
		t1 = RIJNDAEL_ROUND_ENCRYPT(s1, s2, s3, s0, key[5]);
		t2 = RIJNDAEL_ROUND_ENCRYPT(s2, s3, s0, s1, key[6]);
		t3 = RIJNDAEL_ROUND_ENCRYPT(s3, s0, s1, s2, key[7]);
		key += 8;

		if (!--r) {
			break;
		}

		s0 = RIJNDAEL_ROUND_ENCRYPT(t0, t1, t2, t3, key[0]);
		s1 = RIJNDAEL_ROUND_ENCRYPT(t1, t2, t3, t0, key[1]);
		s2 = RIJNDAEL_ROUND_ENCRYPT(t2, t3, t0, t1, key[2]);
		s3 = RIJNDAEL_ROUND_ENCRYPT(t3, t0, t1, t2, key[3]);
	}

	s0 = RIJNDAEL_ROUND_FINISH(table_encrypt_4, t0, t1, t2, t3, key[0]);
	s1 = RIJNDAEL_ROUND_FINISH(table_encrypt_4, t1, t2, t3, t0, key[1]);
	s2 = RIJNDAEL_ROUND_FINISH(table_encrypt_4, t2, t3, t0, t1, key[2]);
	s3 = RIJNDAEL_ROUND_FINISH(table_encrypt_4, t3, t0, t1, t2, key[3]);

	block[0] = LS_SWAP_ENDIAN_BIG_32(s0);
	block[1] = LS_SWAP_ENDIAN_BIG_32(s1);
	block[2] = LS_SWAP_ENDIAN_BIG_32(s2);
	block[3] = LS_SWAP_ENDIAN_BIG_32(s3);

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_rijndael_decrypt_block(const ls_rijndael_t *const LS_RESTRICT ctx, uint32_t *const LS_RESTRICT block) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(block, 2);
	LS_RESULT_CHECK_NULL(ctx->key_decrypt, 3);

	const uint32_t *key = ((uint32_t*)ctx->key_decrypt->data);

	register uint32_t
		s0, s1, s2, s3,
		t0, t1, t2, t3;

	s0 = (LS_SWAP_ENDIAN_BIG_32(block[0]) ^ key[0]);
	s1 = (LS_SWAP_ENDIAN_BIG_32(block[1]) ^ key[1]);
	s2 = (LS_SWAP_ENDIAN_BIG_32(block[2]) ^ key[2]);
	s3 = (LS_SWAP_ENDIAN_BIG_32(block[3]) ^ key[3]);

	ls_nword_t r = (ctx->rounds >> 1);
	for (;;) {
		t0 = RIJNDAEL_ROUND_DECRYPT(s0, s3, s2, s1, key[4]);
		t1 = RIJNDAEL_ROUND_DECRYPT(s1, s0, s3, s2, key[5]);
		t2 = RIJNDAEL_ROUND_DECRYPT(s2, s1, s0, s3, key[6]);
		t3 = RIJNDAEL_ROUND_DECRYPT(s3, s2, s1, s0, key[7]);
		key += 8;

		if (!--r) {
			break;
		}

		s0 = RIJNDAEL_ROUND_DECRYPT(t0, t3, t2, t1, key[0]);
		s1 = RIJNDAEL_ROUND_DECRYPT(t1, t0, t3, t2, key[1]);
		s2 = RIJNDAEL_ROUND_DECRYPT(t2, t1, t0, t3, key[2]);
		s3 = RIJNDAEL_ROUND_DECRYPT(t3, t2, t1, t0, key[3]);
	}

	s0 = RIJNDAEL_ROUND_FINISH(table_decrypt_4, t0, t3, t2, t1, key[0]);
	s1 = RIJNDAEL_ROUND_FINISH(table_decrypt_4, t1, t0, t3, t2, key[1]);
	s2 = RIJNDAEL_ROUND_FINISH(table_decrypt_4, t2, t1, t0, t3, key[2]);
	s3 = RIJNDAEL_ROUND_FINISH(table_decrypt_4, t3, t2, t1, t0, key[3]);

	block[0] = LS_SWAP_ENDIAN_BIG_32(s0);
	block[1] = LS_SWAP_ENDIAN_BIG_32(s1);
	block[2] = LS_SWAP_ENDIAN_BIG_32(s2);
	block[3] = LS_SWAP_ENDIAN_BIG_32(s3);

	return LS_RESULT_SUCCESS;
}
