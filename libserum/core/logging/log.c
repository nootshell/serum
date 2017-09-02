/*******************************************************************************
**                                                                            **
**   The MIT License                                                          **
**                                                                            **
**   Copyright 2017 icecubetray                                               **
**                                                                            **
**   Permission is hereby granted, free of charge, to any person              **
**   obtaining a copy of this software and associated documentation files     **
**   (the "Software"), to deal in the Software without restriction,           **
**   including without limitation the rights to use, copy, modify, merge,     **
**   publish, distribute, sublicense, and/or sell copies of the Software,     **
**   and to permit persons to whom the Software is furnished to do so,        **
**   subject to the following conditions:                                     **
**                                                                            **
**   The above copyright notice and this permission notice shall be           **
**   included in all copies or substantial portions of the Software.          **
**                                                                            **
**   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          **
**   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       **
**   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   **
**   IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     **
**   CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     **
**   TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        **
**   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   **
**                                                                            **
********************************************************************************
**
**  Notes:
**    -
**
*/

#define FILE_PATH							"core/logging/log.c"

#include "./log.h"
#include "../exit.h"
#include "../time.h"
#include "../../runtime/mutex.h"
#include <stdio.h>
#include <stdarg.h>
#include <inttypes.h>


ID("centralized logging");


static ls_log_level_t global_log_level = LS_LOG_DEBUG;
static const char *log_level_tags[] = {
	"",
	LS_ANSI_1(LS_ANSI_FG_BRIGHT_RED) "ERR" LS_ANSI_RESET,
	LS_ANSI_1(LS_ANSI_FG_YELLOW) "WRN" LS_ANSI_RESET,
	LS_ANSI_1(LS_ANSI_FG_BRIGHT_BLUE) "INF" LS_ANSI_RESET,
	LS_ANSI_1(LS_ANSI_FG_DARK_GRAY) "VER" LS_ANSI_RESET,
	LS_ANSI_1(LS_ANSI_FG_PINK) "DBG" LS_ANSI_RESET,
	NULL
};
static size_t log_level_tag_count = 0;

static ls_mutex_t log_lock;
static ls_bool done_init = false;


void
ls_log(ls_log_level_t level, const char *fmt, ...) {
	if (level > global_log_level) {
		return;
	}

	if (!done_init) {
		if (!ls_mutex_init(&log_lock).success) {
			ls_fatal(1, "Failed to initialize logging mutex.");
		}

		for (; log_level_tags[++log_level_tag_count];) {
			;
		}

		done_init = true;
	}

	if (level < 0 || level >= log_level_tag_count) {
		return;
	}

	if (!ls_mutex_lock(&log_lock).success) {
		ls_fatal(1, "Failed to lock logging mutex.");
	}

	/* Time printing scope. */ {
		time_t twhole = ls_secs();
		struct tm tbroken;
		if (localtime_r(&twhole, &tbroken) != NULL) {
			printf("%04u-%02u-%02u %02u:%02u:%02u", (1900 + tbroken.tm_year), tbroken.tm_mon, tbroken.tm_mday, tbroken.tm_hour, tbroken.tm_min, tbroken.tm_sec);
		} else {
			printf("%" PRIu64, twhole);
		}
	}

	printf(" [%s] > ", log_level_tags[level]);

	va_list vl;
	va_start(vl, fmt);
	vprintf(fmt, vl);
	va_end(vl);

	puts("");

	if (!ls_mutex_unlock(&log_lock).success) {
		ls_fatal(1, "Failed to unlock logging mutex.");
	}
}


void
ls_log_set_level(ls_log_level_t level) {
	global_log_level = level;
}


ls_log_level_t
ls_log_get_level() {
	return global_log_level;
}


ls_bool
ls_can_log(ls_log_level_t level) {
	return (level <= global_log_level);
}
