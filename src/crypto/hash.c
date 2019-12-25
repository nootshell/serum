#include "./hash.h"

#include "../core/memory.h"

#include "./hashing/debug.h"
#include "./hashing/md5.h"




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
