/******************************************************************************
**                                                                           **
**   The MIT License                                                         **
**                                                                           **
**   Copyright 2017-2018 icecubetray                                         **
**                                                                           **
**   Permission is hereby granted, free of charge, to any person             **
**   obtaining a copy of this software and associated documentation files    **
**   (the "Software"), to deal in the Software without restriction,          **
**   including without limitation the rights to use, copy, modify, merge,    **
**   publish, distribute, sublicense, and/or sell copies of the Software,    **
**   and to permit persons to whom the Software is furnished to do so,       **
**   subject to the following conditions:                                    **
**                                                                           **
**   The above copyright notice and this permission notice shall be          **
**   included in all copies or substantial portions of the Software.         **
**                                                                           **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,         **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF      **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY    **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,    **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE       **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                  **
**                                                                           **
******************************************************************************/


#include "./base.h"

#include "../hash_registry.h"

#include "../../core/memory.h"
#include "../../data/time.h"
#include "../../io/ansi-ctrl.h"
#include "../../io/log.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>




FILEID("Cryptographic selftesting (CST) base.");




static ls_bool_t __logging = false;




void
lscst_set_logging(const ls_bool_t enabled) {
	__logging = !!enabled;
}




ls_result_t
lscst_launch() {
	if (__hash_registry_count == 0) {
		return LS_E_NOOP;
	}


	ls_result_t result = LS_E_SUCCESS;


	ls_log_writeln(NULL, LS_LOG_LEVEL_INFO, LS_ANSI_WRAP("Performing CSTs.", LS_ANSI_FG_ORANGE, LS_ANSI_OPT_ITALIC));


	uint64_t ns = 0;
	ls_result_t st_result = LS_E_NOOP;

	register size_t i;
	const lsreg_hash_t *hash = NULL;
	const lsreg_meta_t *meta = NULL;
	for (i = 1; i < __hash_registry_count; ++i) {
		hash = &__hash_registry[i];
		meta = &hash->meta;

		if (meta->selftest == NULL) {
			if (__logging) {
				ls_log_writeln(NULL, LS_LOG_LEVEL_WARNING, LS_ANSI_WRAP(">", LS_ANSI_FG_MAGENTA) " " LS_ANSI_WRAP("%s", LS_ANSI_FG_WHITE) " (ai=%02" PRIXPTR "): " LS_ANSI_WRAP("no CST entrypoint set!", LS_ANSI_FG_YELLOW), meta->name, i);
			}
			continue;
		}

		if (__logging) {
			ns = ls_time_nanos();
		}

		if ((st_result = meta->selftest(meta)) != LS_E_SUCCESS) {
			result = LS_E_FAILURE;
		}

		if (__logging) {
			ns = (ls_time_nanos() - ns);

			ls_log_writeln(
				NULL,
				((st_result == LS_E_SUCCESS) ? LS_LOG_LEVEL_INFO : LS_LOG_LEVEL_SEVERE),
				LS_ANSI_WRAP(">", LS_ANSI_FG_TEAL) " " LS_ANSI_WRAP("%s", LS_ANSI_FG_WHITE) " (ai=%02" PRIXPTR "): %s, took ~" LS_ANSI_WRAP("%" PRIu64, LS_ANSI_FG_WHITE) " μs",
				meta->name, i, ((st_result == LS_E_SUCCESS) ? LS_ANSI_WRAP("passed", LS_ANSI_FG_GREEN) : LS_ANSI_WRAP("failed", LS_ANSI_FG_RED)), (ns / 1000)
			);
		}
	}


	ls_log_writeln(NULL, LS_LOG_LEVEL_INFO, LS_ANSI_WRAP("Done performing CSTs.", LS_ANSI_FG_ORANGE, LS_ANSI_OPT_ITALIC));


	return result;
}




void
lscst_log(const ls_result_t result, const char *const algorithm, const size_t index, const char *const source, const uint8_t *const data_expected, const uint8_t *const data_found, const size_t data_size) {
	if (__logging) {
		const ls_log_level_t level = ((result == LS_E_SUCCESS) ? LS_LOG_LEVEL_INFO : LS_LOG_LEVEL_SEVERE);
		ls_log_writeln(
			NULL,
			level,
			LS_ANSI_WRAP(">", LS_ANSI_FG_TEAL) " " LS_ANSI_WRAP("%s", LS_ANSI_FG_WHITE) ": " LS_ANSI_WRAP("%s", LS_ANSI_FG_RED) " (ti=%02" PRIuPTR ", src=\"%s\")",
			algorithm, ls_result_string(result), index, source
		);

		if (data_expected != NULL && data_found != NULL && data_size > 0) {
			ls_log_writeln(
				NULL,
				level,
				"   > Expected %s",
				ls_memory_to_c_array(data_expected, data_size)
			);
			ls_log_writeln(
				NULL,
				level,
				"   > Found    %s",
				ls_memory_to_c_array(data_found, data_size)
			);
		}
	}
}
