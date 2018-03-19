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

#include "../../io/ansi-ctrl.h"
#include "../../io/log.h"
#include "../../data/time.h"
#include "../hash_registry.h"

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

	struct lsreg_hashing *item;
	struct lscst_entry *item_st;

	uint64_t ns = 0;
	size_t i;
	for (i = 0; i < __hash_registry_count; ++i) {
		item = &__hash_registry[i];
		item_st = &item->selftest;

		if (item_st->entrypoint == NULL) {
			continue;
		}

		if (__logging) {
			ns = ls_time_nanos();
		}

		if ((item_st->result = item_st->entrypoint(item_st)) != LS_E_SUCCESS) {
			result = LS_E_FAILURE;
		}

		if (__logging) {
			ns = (ls_time_nanos() - ns);
			ls_log_writeln(
				NULL,
				LS_LOG_LEVEL_INFO,
				"Ran %s in %" PRIu64 "ns: %s",
				item->name,
				ns,
				((item_st->result == LS_E_SUCCESS) ? "passed" : "failed")
			);
		}
	}


	if (__logging) {
		size_t f;
		ls_bool_t success;
		ls_log_level_t level;
		for (i = 0; i < __hash_registry_count; ++i) {
			item = &__hash_registry[i];
			item_st = &item->selftest;


			success = (item_st->failures == NULL && item_st->n_failures == 0);
			level = (success ? LS_LOG_LEVEL_INFO : LS_LOG_LEVEL_SEVERE);


			ls_log_writeln(NULL, level, "%s:", item->name);


			ls_log_writeln(NULL, level, "  Result:");

			if (success) {
				ls_log_writeln(NULL, level, "    " LS_ANSI_WRAP(LS_ANSI_FG_GREEN, "%s"), "No failures.");
				continue;
			}

			for (f = 0; f < item_st->n_failures; ++f) {
				ls_log_writeln(NULL, level, ">   " LS_ANSI_WRAP(LS_ANSI_FG_RED, "%s"), item_st->failures[f]);
				free(item_st->failures[f]);
			}

			free(item_st->failures);
			item_st->failures = NULL;
		}
	}


	return result;
}



ls_result_t
lscst_report_failure(void *const __st, const char *const mesg) {
	if (__st == NULL || mesg == NULL) {
		return LS_E_NULL;
	}

	if (!__logging) {
		return LS_E_NOOP;
	}


	const size_t mesglen = strlen(mesg);
	char *str = malloc(mesglen + 1);
	if (str == NULL) {
		return LS_E_MEMORY;
	}
	strncpy(str, mesg, mesglen);


	struct lscst_entry *item = __st;
	if (item->failures == NULL) {
		item->n_failures = 0;
	}


	char **ptr = realloc(item->failures, (sizeof(*item->failures) * (item->n_failures + 1)));
	if (ptr == NULL) {
		free(str);
		return LS_E_MEMORY;
	}
	item->failures = ptr;

	item->failures[item->n_failures++] = str;

	return LS_E_SUCCESS;
}
