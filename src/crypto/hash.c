#include "./hash.h"

#include "./hashing/md5.h"




/* Temporary substitution for upcoming in-section registration. */

struct serum_hash {
	serum_interface_hash_init f_init;
	serum_interface_hash_clear f_clear;
	serum_interface_hash_update f_update;
	serum_interface_hash_finish f_finish;

	unsigned int identifier;

	char name[16];
};

const static struct serum_hash __hash_register[] = {
	{
		.f_init = (serum_interface_hash_init)serum_md5_init,
		.f_clear = (serum_interface_hash_clear)serum_md5_clear,
		.f_update = (serum_interface_hash_update)serum_md5_update,
		.f_finish = (serum_interface_hash_finish)serum_md5_finish,

		.identifier = SERUM_MD5,

		.name = "MD5"
	}
};




unsigned int
serum_hash_getimpl(const unsigned int identifier, serum_interface_hash_init *const out_init, serum_interface_hash_clear *const out_clear, serum_interface_hash_update *const out_update, serum_interface_hash_finish *const out_finish) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(out_init);
		SERUM_CHECK_NULLPTR(out_clear);
		SERUM_CHECK_NULLPTR(out_update);
		SERUM_CHECK_NULLPTR(out_finish);
	);

	unsigned int register i;
	const struct serum_hash *a;
	for (i = (sizeof(__hash_register) / sizeof(*__hash_register)); i--;) {
		a = &__hash_register[i];

		if (a->identifier != identifier) {
			continue;
		}

		*out_init = a->f_init;
		*out_clear = a->f_clear;
		*out_update = a->f_update;
		*out_finish = a->f_finish;
		return SERUM_OK;
	}

	return SERUM_ALGORITHM;
}
