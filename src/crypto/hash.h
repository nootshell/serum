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
typedef unsigned int (*serum_interface_hash_init)(void *);


/*!
 * \brief Clears the context referenced by \p context.
 *
 * \param[inout] context Pointer to the instance context.
 *
 * \return
 *		`#SERUM_NULLPTR` if \p context is `NULL` \n
 *		`#SERUM_OK` otherwise.
 */
typedef unsigned int (*serum_interface_hash_clear)(void *);


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
	void *const SATTR_RESTRICT block,
	unsigned int block_remain,
	unsigned long long int total_bytes
);




#endif
