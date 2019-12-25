#ifndef __SERUM_CRYPTO_HASHING_DEBUG_H
#define __SERUM_CRYPTO_HASHING_DEBUG_H




#include "../../core/common.h"




#define SERUM_HASH_DEBUG					0xA0
#define SERUM_HASH_DEBUG_BLOCK_SIZE			64
#define SERUM_HASH_DEBUG_DIGEST_SIZE		0




#ifdef __cplusplus
extern "C" {
#endif

unsigned int serum_hash_debug_init(void *const ctx);
unsigned int serum_hash_debug_clear(void *const ctx);
unsigned int serum_hash_debug_update(void *const SATTR_RESTRICT ctx, const void *const SATTR_RESTRICT block);
unsigned int serum_hash_debug_finish(void *const SATTR_RESTRICT ctx, unsigned char *const SATTR_RESTRICT digest, const void *const SATTR_RESTRICT block, const unsigned int remain, const unsigned long long int total_bytes);

#ifdef __cplusplus
}
#endif




#endif
