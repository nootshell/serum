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

#include "./hashing/md5base.h"
#include "../../io/ansi-ctrl.h"
#include "../../io/log.h"
#include "../../data/string-utils.h"
#include "../../data/time.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>




FILEID("Cryptographic selftesting (CST) base.");




struct cstreg_entry {
	ls_result_t (*entrypoint)(void *const __st);
	char **failures;
	size_t n_failures;
	char name[16];
	char description[52];
	ls_result_t result;
};

static struct cstreg_entry *__cstreg = NULL;
static size_t __cstreg_count = 0;
static ls_bool_t __logging = false;




ls_result_t
lscst_init() {
	if (__cstreg != NULL) {
		return LS_E_ALREADY;
	}

	__cstreg = malloc(0);
	if (__cstreg == NULL) {
		return LS_E_MEMORY;
	}

	__cstreg_count = 0;
	return LS_E_SUCCESS;
}

void
lscst_set_logging(const ls_bool_t enabled) {
	__logging = !!enabled;
}




ls_result_t
lscst_register(const char *const name, const char *const description, ls_result_t(*entrypoint)(void *const __st)) {
	if (entrypoint == NULL) {
		return LS_E_NULL;
	}

	if (__cstreg == NULL) {
		return LS_E_STATE;
	}


	struct cstreg_entry item = {
		.result = 0,
		.entrypoint = entrypoint,
		.failures = NULL,
		.n_failures = 0
	};


	if (name != NULL) {
		ls_strncpy(item.name, name, (sizeof(item.name) - 1));
	} else {
		memset(item.name, 0, sizeof(item.name));
	}

	if (description != NULL) {
		ls_strncpy(item.description, description, (sizeof(item.description) - 1));
	} else {
		memset(item.description, 0, sizeof(item.description));
	}


	const size_t index = __cstreg_count;

	void *const newp = realloc(__cstreg, (sizeof(*__cstreg) * (index + 1)));
	if (newp == NULL) {
		return LS_E_MEMORY;
	} else {
		__cstreg = newp;
		++__cstreg_count;
	}

	memcpy(&__cstreg[index], &item, sizeof(*__cstreg));


	return LS_E_SUCCESS;
}




ls_result_t
lscst_launch() {
	if (__cstreg == NULL) {
		return LS_E_STATE;
	}

	if (__cstreg_count == 0) {
		return LS_E_NOOP;
	}


	ls_result_t result = LS_E_SUCCESS;

	struct cstreg_entry *item;

	uint64_t ns = 0;
	size_t i;
	for (i = 0; i < __cstreg_count; ++i) {
		item = &__cstreg[i];

		if (item->entrypoint == NULL) {
			continue;
		}

		if (__logging) {
			ns = ls_time_nanos();
		}

		if ((item->result = item->entrypoint(item)) != LS_E_SUCCESS) {
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
				((item->result == LS_E_SUCCESS) ? "passed" : "failed")
			);
		}
	}


	if (__logging) {
		size_t f;
		ls_bool_t success;
		ls_log_level_t level;
		for (i = 0; i < __cstreg_count; ++i) {
			item = &__cstreg[i];

			success = (item->failures == NULL && item->n_failures == 0);
			level = (success ? LS_LOG_LEVEL_INFO : LS_LOG_LEVEL_SEVERE);

			ls_log_writeln(NULL, level, "%s:", item->name);
			if (item->description[0] != '\0') {
				ls_log_writeln(NULL, level, "  Description: %s", item->description);
			}

			ls_log_writeln(NULL, level, "  Result:");

			if (success) {
				ls_log_writeln(NULL, level, "    " LS_ANSI_WRAP(LS_ANSI_FG_GREEN, "%s"), "No failures.");
				continue;
			}

			for (f = 0; f < item->n_failures; ++f) {
				ls_log_writeln(NULL, level, ">   " LS_ANSI_WRAP(LS_ANSI_FG_RED, "%s"), item->failures[f]);
				free(item->failures[f]);
			}

			free(item->failures);
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


	struct cstreg_entry *item = __st;
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
