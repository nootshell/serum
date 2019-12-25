#include "./hash.h"

#include "../core/memory.h"

#include "./hashing/debug.h"
#include "./hashing/md5.h"

#include <string.h>




static unsigned int __runtime_checks = 1;




/* Temporary substitution for upcoming in-section registration. */

const static struct serum_hash_info __hash_register[] = {
	/* Debug */ {
		.f_init = serum_hash_debug_init,
		.f_clear = serum_hash_debug_clear,
		.f_update = serum_hash_debug_update,
		.f_finish = serum_hash_debug_finish,

		.identifier = SERUM_HASH_DEBUG,

		.block_size = SERUM_HASH_DEBUG_BLOCK_SIZE,
		.digest_size = SERUM_HASH_DEBUG_DIGEST_SIZE,
		.context_size = 0,

		.name = "DEBUG"
	},
	/* MD5 */ {
		.f_init = (serum_interface_hash_init)serum_md5_init,
		.f_clear = (serum_interface_hash_clear)serum_md5_clear,
		.f_update = (serum_interface_hash_update)serum_md5_update,
		.f_finish = (serum_interface_hash_finish)serum_md5_finish,

		.identifier = SERUM_MD5,

		.block_size = SERUM_MD5_BLOCK_SIZE,
		.digest_size = SERUM_MD5_DIGEST_SIZE,
		.context_size = sizeof(struct serum_md5_context),

		.name = "MD5"
	}
};




unsigned int
serum_hash_getinfo(const unsigned int identifier, struct serum_hash_info *const out_info) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(out_info);
	);

	unsigned int register i;
	const struct serum_hash_info *a;
	for (i = (sizeof(__hash_register) / sizeof(*__hash_register)); i--;) {
		a = &__hash_register[i];

		if (a->identifier != identifier) {
			continue;
		}

		*out_info = *a;
		return SERUM_OK;
	}

	return SERUM_EALGORITHM;
}


unsigned int
serum_hash_getimpl(const unsigned int identifier, serum_interface_hash_init *const out_init, serum_interface_hash_clear *const out_clear, serum_interface_hash_update *const out_update, serum_interface_hash_finish *const out_finish) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(out_init);
		SERUM_CHECK_NULLPTR(out_clear);
		SERUM_CHECK_NULLPTR(out_update);
		SERUM_CHECK_NULLPTR(out_finish);
	);

	struct serum_hash_info info;
	const unsigned int res = serum_hash_getinfo(identifier, &info);
	if (res != SERUM_OK) {
		return res;
	}

	*out_init = info.f_init;
	*out_clear = info.f_clear;
	*out_update = info.f_update;
	*out_finish = info.f_finish;
	return SERUM_OK;
}




unsigned int
serum_hash_init(struct serum_hash *const ctx, const unsigned int identifier) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(ctx);
	);

	unsigned int res;

	res = serum_hash_getinfo(identifier, &ctx->info);
	if (res != SERUM_OK) {
		return res;
	}

	SERUM_SANITY_AREA(
		if (__runtime_checks) {
			SERUM_CHECK_RANGE(ctx->info.context_size, 0, sizeof(ctx->context));
			SERUM_CHECK_RANGE(ctx->info.block_size, 0, sizeof(ctx->buffer));
			SERUM_CHECK_NULLPTR(ctx->info.f_init);
			SERUM_CHECK_NULLPTR(ctx->info.f_clear);
			SERUM_CHECK_NULLPTR(ctx->info.f_update);
			SERUM_CHECK_NULLPTR(ctx->info.f_finish);
		}
	);

	ctx->length = 0;
	ctx->buffer_fill = 0;

	return ctx->info.f_init(ctx->context);
}


unsigned int
serum_hash_clear(struct serum_hash *const ctx) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(ctx);

		if (__runtime_checks) {
			SERUM_CHECK_NULLPTR(ctx->info.f_clear);
		}
	);

	const unsigned int res = ctx->info.f_clear(ctx->context);
	if (res != SERUM_OK) {
		return res;
	}

	serum_erase(ctx, sizeof(*ctx));

	return SERUM_OK;
}




unsigned int
serum_hash_update(struct serum_hash *const SATTR_RESTRICT ctx, const void *const SATTR_RESTRICT data, const size_t data_length) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(ctx);
		SERUM_CHECK_NULLPTR(data);
	);


	/* Update total message length. */
	ctx->length += data_length;


	/* Pre-dereference. */
	const unsigned int block_size = ctx->info.block_size;
	const unsigned int buffer_fill = ctx->buffer_fill;


	/* True length of what we need to process. */
	unsigned int register length = (buffer_fill + data_length);

	/* Check if we even need to do any updates. If not, simply append to buffer and return. */
	if (length < block_size) {
		if (data_length > 0) {
			memcpy(&ctx->buffer[buffer_fill], data, data_length);
			ctx->buffer_fill += data_length;
		}

		return SERUM_OK;
	}


	/* We're going to do work, create a local block-sized buffer. */
	unsigned char block[block_size];

	/* First block is filled hybridly from the cache and direct input data. */
	unsigned int register data_idx = (block_size - buffer_fill);
	memcpy(block, ctx->buffer, buffer_fill);
	memcpy(&block[buffer_fill], data, data_idx);


	/* Pre-dereference function pointer and context. */
	const serum_interface_hash_update f_update = ctx->info.f_update;
	void *const sub_context = ctx->context;

	/* Process input blocks until no full blocks remain. */
	unsigned int register result;
	for (;;) {
		result = f_update(sub_context, block);
		if (result != SERUM_OK) {
			return result;
		}

		if ((length -= block_size) >= block_size) {
			memcpy(block, &((unsigned char *)data)[data_idx], block_size);
			data_idx += block_size;
		} else {
			break;
		}
	}


	/* Erase buffers to prevent leaking. */
	serum_erase(block, sizeof(block));
	serum_erase(ctx->buffer, sizeof(ctx->buffer));


	/* Check if we have any data left we need to buffer for next runs. */
	if (length > 0) {
		memcpy(ctx->buffer, &((unsigned char *)data)[data_idx], length);
		ctx->buffer_fill = length;
	}


	return SERUM_OK;
}




unsigned int
serum_hash_finish(struct serum_hash *const SATTR_RESTRICT ctx, unsigned char *const SATTR_RESTRICT digest) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(ctx);
		SERUM_CHECK_NULLPTR(digest);
	);

	const unsigned int res = ctx->info.f_finish(ctx->context, digest, ctx->buffer, ctx->buffer_fill, ctx->length);
	if (res != SERUM_OK) {
		return res;
	}

	return SERUM_OK;
}




unsigned int
serum_hash(const unsigned int identifier, unsigned char *const SATTR_RESTRICT digest, const void *const SATTR_RESTRICT data, const size_t data_length) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(digest);
		SERUM_CHECK_NULLPTR(data);
	);

	struct serum_hash lux;

	unsigned int result = serum_hash_init(&lux, identifier);
	if (result != SERUM_OK) {
		goto __cleanup;
	}

	result = serum_hash_update(&lux, data, data_length);
	if (result != SERUM_OK) {
		goto __cleanup;
	}

	result = serum_hash_finish(&lux, digest);

__cleanup:
	serum_hash_clear(&lux);
	return result;
}
