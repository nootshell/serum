#ifndef __SERUM_CRYPTO_HASHING_MD5_H
#define __SERUM_CRYPTO_HASHING_MD5_H




#include "../../core/common.h"




#define SERUM_MD5							0xA1	/*!< \hideinitializer \brief MD5 algorithm identifier. */
#define SERUM_MD5_BLOCK_SIZE				64		/*!< \hideinitializer \brief MD5 block size, in bytes. */
#define SERUM_MD5_DIGEST_SIZE				16		/*!< \hideinitializer \brief MD5 digest size, in bytes. */




/*!
 * \brief MD5 context, retains digest state.
 */
struct serum_md5_context {
	uint32_t state[4];
};




#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \copydoc serum_interface_hash_init
 */
unsigned int serum_md5_init(struct serum_md5_context *const ctx);

/*!
 * \copydoc serum_interface_hash_clear
 */
unsigned int serum_md5_clear(struct serum_md5_context *const ctx);

/*!
 * \copydoc serum_interface_hash_update
 */
unsigned int serum_md5_update(struct serum_md5_context *const SATTR_RESTRICT ctx, const void *const SATTR_RESTRICT block);

/*!
 * \copydoc serum_interface_hash_finish
 */
unsigned int serum_md5_finish(struct serum_md5_context *const SATTR_RESTRICT ctx, unsigned char *const SATTR_RESTRICT digest, const void *const SATTR_RESTRICT block, const unsigned int remain, const unsigned long long int total_bytes);

#ifdef __cplusplus
}
#endif




#endif
