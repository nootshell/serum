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

#define FILE_PATH							"networking/socket.c"

#include "./socket.h"


ID("universal socket");


#if (LS_WINDOWS)
#	include <WinSock2.h>
#	include <WS2tcpip.h>

static ls_bool wsaStartup = false;
static WSADATA wsaData;
static size_t num_init_sockets = 0;
#endif


static inline ls_bool close_socket(ls_socket_t *const ctx) {
	if (
#if (LS_WINDOWS)
		closesocket(ctx->fd)
#else
		close(ctx->fd)
#endif
	/*close*/) {
		return false;
	}

	ctx->fd = LS_INVALID_SOCKET;

	return true;
}


ls_result_t
ls_socket_init_ex(ls_socket_t *const ctx, char const *node, uint32_t const flags) {
	if (!ctx) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (!node) {
		node = "0.0.0.0";
	}


	uint32_t err;


#if (LS_WINDOWS)
	if (!wsaStartup) {
		if (!(err = WSAStartup(MAKEWORD(2, 2), &wsaData))) {
			wsaStartup = true;
		} else {
			return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
		}
	}
	++num_init_sockets;
#endif


	ctx->addrinfo = NULL;
	ctx->fd = LS_INVALID_SOCKET;
	ctx->flags = flags;


	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = (HAS_FLAG(ctx->flags, LS_SOCKET_UDP) ? SOCK_DGRAM : SOCK_STREAM);
	hints.ai_flags = (HAS_FLAG(ctx->flags, LS_SOCKET_SERVER) ? AI_PASSIVE : 0);
	hints.ai_protocol = 0;


	if ((err = getaddrinfo(node, NULL, &hints, &ctx->addrinfo))) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}


	return LS_RESULT_SUCCESS;
}

ls_result_t
ls_socket_init(ls_socket_t *const ctx, char const *node) {
	return ls_socket_init_ex(ctx, node, 0);
}


ls_result_t
ls_socket_clear(ls_socket_t *const ctx) {
	if (!ctx) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (ctx->fd != LS_INVALID_SOCKET) {
		if (!close_socket(ctx)) {
			return LS_RESULT_ERROR(LS_RESULT_CODE_CLOSE);
		}
	}

	if (ctx->addrinfo) {
		freeaddrinfo(ctx->addrinfo);
		ctx->addrinfo = NULL;
	}

#if (LS_WINDOWS)
	if (!num_init_sockets) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_UNSUPPORTED);
	} else if (num_init_sockets == 1) {
		if (wsaStartup) {
			if (WSACleanup() == 0) {
				wsaStartup = false;
			} else {
				return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
			}
		} else {
			return LS_RESULT_ERROR(LS_RESULT_CODE_DATA);
		}
	}
	--num_init_sockets;
#endif

	return LS_RESULT_SUCCESS;
}
