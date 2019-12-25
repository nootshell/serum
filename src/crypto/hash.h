#ifndef __SERUM_CRYPTO_HASH_H
#define __SERUM_CRYPTO_HASH_H




#include "../core/common.h"




/*!
 * \brief Initializes the context referenced by \p context.
 *
 * \param[inout] context Pointer to the instance context.
 *
 * \return
 *		`#SERUM_NULLPTR` if \p context is `NULL` \n
 *		`#SERUM_OK` otherwise.
 */
typedef unsigned int (*serum_interface_hash_init)(
	void *const context
);


/*!
 * \brief Clears the context referenced by \p context.
 *
 * \param[inout] context Pointer to the instance context.
 *
 * \return
 *		`#SERUM_NULLPTR` if \p context is `NULL` \n
 *		`#SERUM_OK` otherwise.
 */
typedef unsigned int (*serum_interface_hash_clear)(
	void *const context
);


/*!
 * \brief Perform block transformation of \p block and apply it to \p context.
 *
 * \param[inout] context Pointer to the instance context.
 * \param[in]    block Block to transform.
 *
 * \return
 *		`#SERUM_NULLPTR` if \p context or \p block is `NULL`, \n
 *		`#SERUM_OK` otherwise.
 */
typedef unsigned int (*serum_interface_hash_update)(
	void *const SATTR_RESTRICT context,
	const void *const SATTR_RESTRICT block
);


/*!
 * \brief Calculates the \p digest for the specified \p context.
 *
 * Finalizes the digest instance by e.g. padding and appending lengths where appropriate, processing the remaining input. Clears the context upon success if compiled to do so.
 *
 * \param[inout] context Pointer to the instance context.
 * \param[out]   digest The output location of the digest.
 * \param[in]    block Remaining block to transform. May be `NULL` if there is nothing to transform given that \p block_remain is set to `0`.
 * \param[in]    block_remain The length of the remaining block.
 * \param[in]    total_bytes The total length of the message in bytes.
 *
 * \return
 *		`#SERUM_NULLPTR` if any of \p context, \p digest, or \p block is `NULL`, \n
 *		`#SERUM_RANGE` if \p block_remain is out of range (negative, or greater than/equal to the digest block size) \n
 *		`#SERUM_OK` otherwise.
 */
typedef unsigned int (*serum_interface_hash_finish)(
	void *const SATTR_RESTRICT context,
	unsigned char *const SATTR_RESTRICT digest,
	const void *const SATTR_RESTRICT block,
	const unsigned int block_remain,
	const unsigned long long int total_bytes
);




struct serum_hash_info {
	serum_interface_hash_init f_init;
	serum_interface_hash_clear f_clear;
	serum_interface_hash_update f_update;
	serum_interface_hash_finish f_finish;

	unsigned int identifier;
	unsigned int block_size;
	unsigned int digest_size;
	unsigned int context_size;

	char name[16];
};


struct serum_hash {
	struct serum_hash_info info;	/*!< \brief Structure containing algorithm information. */

	unsigned int context[16]; 		/*!< \brief Space reserved for a hash context. */

	unsigned int buffer_fill;		/*!< \brief */
	unsigned char buffer[512];		/*!< \brief */
};




#ifdef __cplusplus
extern "C" {
#endif

unsigned int serum_hash_getinfo(const unsigned int identifier, struct serum_hash_info *const out_info);
unsigned int serum_hash_getimpl(const unsigned int identifier, serum_interface_hash_init *const out_init, serum_interface_hash_clear *const out_clear, serum_interface_hash_update *const out_update, serum_interface_hash_finish *const out_finish);

unsigned int serum_hash_init(struct serum_hash *const ctx, const unsigned int identifier);
unsigned int serum_hash_clear(struct serum_hash *const ctx);
unsigned int serum_hash_update(struct serum_hash *const SATTR_RESTRICT ctx, const void *const SATTR_RESTRICT data, const size_t data_length);

#ifdef __cplusplus
}
#endif




#endif
