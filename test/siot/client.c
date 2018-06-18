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
*******************************************************************************/


#undef LS_EXPORTING

#include "../../libserum/io/log.h"
#include "../../libserum/io/socket.h"

#include <stdlib.h>
#include <string.h>




int
__run_siot_client() {
	ls_socket_t sock;

	if (ls_socket_init(&sock, LS_SOCKET_TCP) != LS_E_SUCCESS) {
		ls_log_writeln(NULL, LS_LOG_LEVEL_ERROR, "Failed to initialize socket");
		return 1;
	}

	sock.mtu = 4;

	if (ls_socket_start(&sock, "::1", NULL, 12345) != LS_E_SUCCESS) {
		ls_log_writeln(NULL, LS_LOG_LEVEL_ERROR, "Failed to start socket");
		return 1;
	}


	char msgbuf[256];

	size_t size, sztotal, szcursor;
	ls_bool_t running = true;
	do {
		if (fgets(msgbuf, sizeof(msgbuf), stdin) == NULL) {
			running = false;
			break;
		}

		size = strlen(msgbuf);
		if (ls_socket_write(&sock, msgbuf, size) != LS_E_SUCCESS) {
			abort();
		}

		sztotal = size;
		szcursor = 0;

		do {
			const ls_result_t result = ls_socket_read(&sock, (msgbuf + szcursor), (sizeof(msgbuf) - szcursor), &size);
			if (result != LS_E_SUCCESS) {
				if (result == LS_E_IO_CLOSE) {
					goto __jmp_ret;
				} else {
					abort();
				}
			}

			szcursor += size;
		} while (szcursor < sztotal);

		msgbuf[sztotal - LS_EOL_SIZE] = '\0';
		puts(msgbuf);
	} while (running);

__jmp_ret:
	return 0;
}
