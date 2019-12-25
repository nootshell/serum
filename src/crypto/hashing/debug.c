#include "./debug.h"

#include "../../core/memory.h"

#include <stdio.h>




unsigned int
serum_hash_debug_init(void *const ctx) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(ctx);
	);

	puts("init");

	return SERUM_OK;
}


unsigned int
serum_hash_debug_clear(void *const ctx) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(ctx);
	);

	puts("deinit");

	return SERUM_OK;
}


unsigned int
serum_hash_debug_update(void *const SATTR_RESTRICT ctx, const void *const SATTR_RESTRICT block) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(ctx);
		SERUM_CHECK_NULLPTR(block);
	);

	puts("");
	puts("update");
	serum_memdump(block, SERUM_HASH_DEBUG_BLOCK_SIZE);
	puts("");

	return SERUM_OK;
}


unsigned int
serum_hash_debug_finish(void *const SATTR_RESTRICT ctx, unsigned char *const SATTR_RESTRICT digest, const void *const SATTR_RESTRICT block, const unsigned int remain, const unsigned long long int total_bytes) {
	SERUM_SANITY_AREA(
		SERUM_CHECK_NULLPTR(ctx);
		SERUM_CHECK_NULLPTR(digest);
		SERUM_CHECK_NULLPTR(block);
		SERUM_CHECK_RANGE(remain, 0, (SERUM_HASH_DEBUG_BLOCK_SIZE - 1));
	);

	puts("");
	printf("finish (r=%u, t=%llu)\n", remain, total_bytes);
	serum_memdump(block, remain);
	puts("");

	return SERUM_OK;
}
