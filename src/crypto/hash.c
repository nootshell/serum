#include "./hash.h"

#include "./hashing/md5.h"




/* Temporary substitution for upcoming in-section registration. */

const static struct serum_hash_info __hash_register[] = {
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
