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

#include <string.h>
#include <arpa/inet.h>




int
__run_siot_server() {
	ls_socket_t sock;

	if (ls_socket_init(&sock, (LS_SOCKET_TCP | LS_SOCKET_SERVER | LS_SOCKOPT_REUSEADDR | LS_SOCKOPT_REUSEPORT)) != LS_E_SUCCESS) {
		ls_log_writeln(NULL, LS_LOG_LEVEL_ERROR, "Failed to initialize socket");
		return 1;
	}

	if (ls_socket_start(&sock, "::1", NULL, 12345) != LS_E_SUCCESS) {
		ls_log_writeln(NULL, LS_LOG_LEVEL_ERROR, "Failed to start socket");
		return 1;
	}


	char buffer[256], c, addrbuf[INET6_ADDRSTRLEN];

	ls_result_t result;
	ls_socket_t client;
	ls_bool_t running = true;
	size_t size, i;
	struct sockaddr addr;
	socklen_t addrlen;
	do {
		ls_log_writeln(NULL, LS_LOG_LEVEL_ERROR, "Ready to accept a client");

		addrlen = sizeof(addr);
		if (ls_socket_accept_ex(&sock, &client, &addr, &addrlen) != LS_E_SUCCESS) {
			ls_log_writeln(NULL, LS_LOG_LEVEL_ERROR, "Failed to accept client");
			continue;
		}

		inet_ntop(addr.sa_family, addr.sa_data, addrbuf, addrlen);
		ls_log_writeln(NULL, LS_LOG_LEVEL_ERROR, "Client accepted: address=[%s]", addrbuf);

		while (((result = ls_socket_read(&client, buffer, sizeof(buffer), &size)) == LS_E_SUCCESS) && size > 0) {
			ls_log_writeln(NULL, LS_LOG_LEVEL_INFO, "Read %" PRIuPTR " bytes from client", size);

			if (size >= 4) {
				if (strncmp("exit", buffer, 4) == 0) {
					ls_log_writeln(NULL, LS_LOG_LEVEL_WARNING, "Exiting");
					running = false;
					break;
				}
			}

			for (i = 0; i < size; ++i) {
				c = buffer[i];

				if (c == 0) {
					goto __break_read_loop;
				}

				/* Simple character case invert. */
				if (c >= 'a' && c <= 'z') {
					c -= 32;
				} else if (c >= 'A' && c <= 'Z') {
					c += 32;
				}

				buffer[i] = c;
			}

			if (ls_socket_write(&client, buffer, size) != LS_E_SUCCESS) {
				ls_log_writeln(NULL, LS_LOG_LEVEL_ERROR, "Failed to write to client");
			} else {
				ls_log_writeln(NULL, LS_LOG_LEVEL_INFO, "Sent %" PRIuPTR " bytes to client", size);
			}
		}
__break_read_loop:

		if (result != LS_E_SUCCESS) {
			ls_log_writeln(NULL, LS_LOG_LEVEL_ERROR, "Failed to read from client");
		}

		if (ls_socket_clear(&client) != LS_E_SUCCESS) {
			ls_log_writeln(NULL, LS_LOG_LEVEL_ERROR, "Failed to close client socket");
		}
	} while (running);


	return 0;
}
