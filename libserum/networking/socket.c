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
**    Needs testing.
**
*/

#define FILE_PATH							"networking/socket.c"

#include "./socket.h"
#include "../core/ptrarithmetic.h"
#include "../core/time.h"
#include "../debug/log.h"
#include <string.h>
#include <inttypes.h>


#define STOP_TIMEOUT_INTERVAL				250


#if (LS_WINDOWS)
#	include <WinSock2.h>
#	include <WS2tcpip.h>

static ls_bool wsaStartup = false;
static WSADATA wsaData;
static size_t num_init_sockets = 0;
#else
#	include <fcntl.h>
#	include <netdb.h>
#	include <unistd.h>
#endif


ID("universal socket");


uint32_t
static inline get_mtu() {
	return 4096;
}

ls_sockfd_t
static inline validate_sockfd(ls_sockfd_t fd, ls_bool *const valid) {
#if (LS_WINDOWS)
	return ((*valid = (fd != INVALID_SOCKET)) ? fd : LS_INVALID_SOCKET);
#else
	return ((*valid = (fd < 0)) ? fd : LS_INVALID_SOCKET);
#endif
}


ls_bool
static inline create_socket(ls_socket_t *const ctx, struct addrinfo *const addr) {
	ls_bool valid = false;

	ctx->fd = validate_sockfd(
		socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol),
		&valid
	);

	return valid;
}

ls_bool
static inline accept_socket(ls_sockfd_t *const fd, const ls_socket_t *const ctx, struct sockaddr *const saddr, socklen_t *const saddrlen) {
	ls_bool valid = false;

	*fd = validate_sockfd(
		accept(ctx->fd, saddr, saddrlen),
		&valid
	);

	if (!valid) {
		perror("accept");
	} else {
		ls_logf("%"PRIu64, (uint64_t)*fd);
	}

	return valid;
}

ls_bool
static inline close_socket(ls_socket_t *const ctx) {
#if (LS_WINDOWS)
	if (closesocket(ctx->fd) != 0) {
		return false;
	}
#else
	if (close(ctx->fd) != 0) {
		return false;
	}
#endif

	ctx->fd = LS_INVALID_SOCKET;
	ctx->selected = NULL;
	ctx->flags = ((ctx->flags | LS_SOCKET_EXITED) & ~(LS_SOCKET_END_READ | LS_SOCKET_END_WRITE));

	return true;
}


ls_result_t
ls_socket_init_ex(ls_socket_t *const ctx, const char *node, const uint32_t flags) {
	if (!ctx) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx null");
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
			ls_log_w(LS_ERRSTR_OPERATION_FAILURE": startup failure");
			return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
		}
	}
	++num_init_sockets;
#endif


	ctx->addrinfo = NULL;
	ctx->selected = NULL;
	ctx->fd = LS_INVALID_SOCKET;
	ctx->flags = flags;
	ctx->mtu = get_mtu();


	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = (HAS_FLAG(ctx->flags, LS_SOCKET_UDP) ? SOCK_DGRAM : SOCK_STREAM);
	hints.ai_flags = (HAS_FLAG(ctx->flags, LS_SOCKET_SERVER) ? AI_PASSIVE : 0);
	hints.ai_protocol = 0;


	if ((err = getaddrinfo(node, NULL, &hints, &ctx->addrinfo))) {
		ls_log_w(LS_ERRSTR_OPERATION_FAILURE": unable to obtain address info");
		return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
	}


	return LS_RESULT_SUCCESS;
}

ls_result_t
ls_socket_init(ls_socket_t *const ctx, const char *node) {
	return ls_socket_init_ex(ctx, node, 0);
}


ls_result_t
ls_socket_clear(ls_socket_t *const ctx) {
	if (!ctx) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx null");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (ctx->fd != LS_INVALID_SOCKET) {
		if (!close_socket(ctx)) {
			ls_log_w(LS_ERRSTR_OPERATION_FAILURE": unable to close socket");
			return LS_RESULT_ERROR(LS_RESULT_CODE_CLOSE);
		}
	}

	if (ctx->addrinfo) {
		freeaddrinfo(ctx->addrinfo);
		ctx->addrinfo = NULL;
	}

	ctx->selected = NULL;

#if (LS_WINDOWS)
	if (!num_init_sockets) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": too many clear calls");
		return LS_RESULT_ERROR(LS_RESULT_CODE_UNSUPPORTED);
	} else if (num_init_sockets == 1) {
		if (wsaStartup) {
			ls_log_d("ws2 cleanup");
			if (WSACleanup() == 0) {
				wsaStartup = false;
			} else {
				ls_log_w(LS_ERRSTR_OPERATION_FAILURE": cleanup failed");
				return LS_RESULT_ERROR(LS_RESULT_CODE_FUNCTION);
			}
		} else {
			ls_log_w(LS_ERRSTR_OPERATION_FAILURE": data failure");
			return LS_RESULT_ERROR(LS_RESULT_CODE_DATA);
		}
	}
	--num_init_sockets;
#endif

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_socket_start(ls_socket_t *const ctx, const uint16_t port) {
	if (!port) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": port null");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_DATA, 1);
	}

	if (!ctx) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx null");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (!ctx->addrinfo) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx->addrinfo null");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 2);
	}

	const uint32_t enable = 1;
	struct addrinfo *ptr;
	uint16_t nport = htons(port);
	for (ptr = ctx->addrinfo; ptr; ptr = ptr->ai_next) {
		((struct sockaddr_in*)ptr->ai_addr)->sin_port = nport;

		if (!create_socket(ctx, ptr)) {
			continue;
		}

		if (HAS_FLAG(ctx->flags, LS_SOCKET_SERVER)) {
#ifdef SO_REUSEADDR
			if (HAS_FLAG(ctx->flags, LS_SOCKET_REUSEADDR)) {
				if (setsockopt(ctx->fd, SOL_SOCKET, SO_REUSEADDR, (void*)&enable, sizeof(enable)) == -1) {
					ls_log_w(LS_ERRSTR_OPTION_UNAVAILABLE": REUSEADDR");
				}
			}
#else
			LS_COMPILER_WARN("LS_SOCKET_REUSEADDR unavailable: SO_REUSEADDR undefined")
#endif
#ifdef SO_REUSEPORT
				if (HAS_FLAG(ctx->flags, LS_SOCKET_REUSEPORT)) {
					if (setsockopt(ctx->fd, SOL_SOCKET, SO_REUSEPORT, (void*)&enable, sizeof(enable)) == -1) {
						ls_log_w(LS_ERRSTR_OPTION_UNAVAILABLE": REUSEPORT");
					}
				}
#else
				LS_COMPILER_WARN("LS_SOCKET_REUSEPORT unavailable: SO_REUSEPORT undefined");
#endif
			if (bind(ctx->fd, ptr->ai_addr,
#if (LS_WINDOWS) // Thanks.
					 (int)
#endif
					 ptr->ai_addrlen) == 0) {
				if (listen(ctx->fd, SOMAXCONN) == 0) {
					break;
				} else {
					ls_log_w(LS_ERRSTR_OPERATION_FAILURE": unable to listen");
				}
			} else {
				ls_log_w(LS_ERRSTR_OPERATION_FAILURE": unable to bind");
			}
		} else {
			if (connect(ctx->fd, ptr->ai_addr,
#if (LS_WINDOWS) // Thanks.
						(int)
#endif
						ptr->ai_addrlen) == 0) {
				break;
			} else {
				ls_log_w(LS_ERRSTR_OPERATION_FAILURE": unable to connect");
			}
		}

		close_socket(ctx);
	}

	ctx->selected = ptr;

	if (!ctx->selected) {
		ctx->fd = LS_INVALID_SOCKET;
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx->selected null");
		return LS_RESULT_ERROR(LS_RESULT_CODE_INITIALIZATION);
	}

	ctx->flags &= ~LS_SOCKET_EXITED;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_socket_stop_ex(ls_socket_t *const ctx, const ls_bool force, const uint_fast16_t timeout) {
	if (!ctx) {
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (ctx->fd == LS_INVALID_SOCKET) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx->fd invalid");
		return LS_RESULT_ERROR(LS_RESULT_CODE_DESCRIPTOR);
	}

#define CHECK								(HAS_FLAG(ctx->flags, LS_SOCKET_END_READ) && HAS_FLAG(ctx->flags, LS_SOCKET_END_WRITE))
	if (!CHECK) {
		if (timeout) {
			uint32_t n = ((timeout / STOP_TIMEOUT_INTERVAL) + 1);

			do {
				if (CHECK) {
					break;
				}
				ls_sleep_millis(STOP_TIMEOUT_INTERVAL);
			} while (--n);

			if (!CHECK) {
				if (!force) {
					ls_log_w(LS_ERRSTR_CHECK_FAILURE": timed out");
					return LS_RESULT_ERROR(LS_RESULT_CODE_TIMEOUT);
				} else {
					ls_log_w(LS_ERRSTR_OPERATION_FAILURE": timed out, forcing shutdown");
				}
			}
		} else {
			if (!force) {
				ls_log_w(LS_ERRSTR_CHECK_FAILURE": timed out");
				return LS_RESULT_ERROR(LS_RESULT_CODE_CHECK);
			} else {
				ls_log_w(LS_ERRSTR_OPERATION_FAILURE": check failed, forcing shutdown");
			}
		}
	}
#undef CHECK

#if(LS_WINDOWS)
#	define SHUTDOWN_FLAGS					SD_BOTH
#else
#	define SHUTDOWN_FLAGS					SHUT_RDWR
#endif
	ls_log_d("shutdown");
	if (shutdown(ctx->fd, SHUTDOWN_FLAGS) != 0) {
		if (!force) {
			ls_log_w(LS_ERRSTR_CHECK_FAILURE": shutdown error");
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_CLOSE, 1);
		} else {
			ls_log_w(LS_ERRSTR_OPERATION_FAILURE": shutdown failed, forcing close");
		}
	}
#undef SHUTDOWN_FLAGS

	return (close_socket(ctx) ? LS_RESULT_SUCCESS : LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_CLOSE, 2));
}

ls_result_t
ls_socket_stop(ls_socket_t *const ctx, const ls_bool force) {
	return ls_socket_stop_ex(ctx, force, 5000);
}


ls_result_t
ls_socket_fromfd(ls_socket_t *const ctx, const ls_sockfd_t fd, const uint32_t flags) {
	if (!ctx) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx null");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (fd == LS_INVALID_SOCKET) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx->fd invalid");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_DESCRIPTOR, 1);
	}

	memset(ctx, 0, sizeof(*ctx));

	ctx->fd = fd;
	ctx->flags = flags;
	ctx->mtu = get_mtu();

	return LS_RESULT_SUCCESS;
}

ls_sockfd_t
ls_socket_acceptfd(const ls_socket_t *const ctx, struct sockaddr *const saddr, socklen_t *const saddrlen) {
	if (!ctx || ctx->fd == LS_INVALID_SOCKET) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx->fd invalid");
		return LS_INVALID_SOCKET;
	}

	ls_sockfd_t fd;

	if (!accept_socket(&fd, ctx, saddr, saddrlen)) {
		ls_log_w(LS_ERRSTR_OPERATION_FAILURE": unable to accept socket");
		return LS_INVALID_SOCKET;
	}

	return fd;
}

ls_result_t
ls_socket_accept(ls_socket_t *const out, const ls_socket_t *const ctx, struct sockaddr *const saddr, socklen_t *const saddrlen) {
	if (!ctx) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx null");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 2);
	}

	if (ctx->fd == LS_INVALID_SOCKET) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx->fd invalid");
		return LS_RESULT_ERROR(LS_RESULT_CODE_DESCRIPTOR);
	}

	ls_result_t res;
	if (!(res = ls_socket_fromfd(out, ls_socket_acceptfd(ctx, saddr, saddrlen), LS_SOCKET_ACCEPTED)).success) {
		ls_log_w(LS_ERRSTR_OPERATION_FAILURE": ls_socket_fromfd failed");
		return res;
	}

	++num_init_sockets;

	if (HAS_FLAG(ctx->flags, LS_SOCKET_ASYNC_CHILDREN)) {
		return ls_socket_set_option(out, LS_SO_ASYNC, 1);
	}

	return LS_RESULT_SUCCESS;
}


ssize_t
static inline safe_send(const ls_socket_t *const ctx, const void *const in, const uint32_t size) {
	ssize_t sent;
	while ((sent = send(ctx->fd, in, size, 0)) == 0) {
		;
	}
	return sent;
}


ls_result_t
ls_socket_write(size_t *const out_size, const ls_socket_t *const ctx, const void *const in, size_t size) {
	if (!ctx) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx null");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (!in) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": input null");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 2);
	}

	if (!size) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": size invalid");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_SIZE, 1);
	}

	if (ctx->fd == LS_INVALID_SOCKET) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx->fd invalid");
		return LS_RESULT_ERROR(LS_RESULT_CODE_DESCRIPTOR);
	}

	ssize_t sent;
	const uint8_t *ptr = in;
	if (size > ctx->mtu) {
		do {
			if ((sent = safe_send(ctx, ptr, ctx->mtu)) > 0) {
				ptr += sent;
				size -= sent;
				if (size <= ctx->mtu) {
					goto __send_remaining;
				}
			} else {
				if (out_size) {
					*out_size = LS_PTR_DIFF(ptr, in);
				}

				return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_WRITE, 1);
			}
		} while (size > ctx->mtu);
	} else {
__send_remaining:
		if ((sent = safe_send(ctx, ptr, (uint32_t)size)) > 0) {
			ptr += sent;
		} else {
			ls_log_w(LS_ERRSTR_OPERATION_FAILURE": writing failure");
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_WRITE, 2);
		}
	}

	if (out_size) {
		*out_size = LS_PTR_DIFF(ptr, in);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_socket_write_str(size_t *const out_size, const ls_socket_t *const ctx, const char *const str) {
	return ls_socket_write(out_size, ctx, str, (str ? (strlen(str) + 1) : 0));
}


ls_result_t
ls_socket_read(size_t *const out_size, const ls_socket_t *const ctx, void *const out, const size_t size) {
	if (!ctx) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx null");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (!out) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": output null");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 2);
	}

	if (!size) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": size invalid");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_SIZE, 1);
	}

	if (ctx->fd == LS_INVALID_SOCKET) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx->fd invalid");
		return LS_RESULT_ERROR(LS_RESULT_CODE_DESCRIPTOR);
	}

	ssize_t received;
	if ((received = recv(ctx->fd, out,
#if (LS_WINDOWS) // Thanks.
						 (int)
#endif
						 size, 0)) > 0) {
		if (out_size) {
			*out_size = (size_t)received;
		}
		return LS_RESULT_SUCCESS;
	}

	return LS_RESULT_ERROR(LS_RESULT_CODE_READ);
}


ls_result_t
ls_socket_set_option(ls_socket_t *const ctx, const enum ls_socket_option_type type, const uint32_t value) {
	if (!ctx) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx null");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_NULL, 1);
	}

	if (!type) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": type null");
		return LS_RESULT_ERROR(LS_RESULT_CODE_EARLY_EXIT);
	}

	if (ctx->fd == LS_INVALID_SOCKET) {
		ls_log_w(LS_ERRSTR_CHECK_FAILURE": ctx->fd invalid");
		return LS_RESULT_ERROR(LS_RESULT_CODE_DESCRIPTOR);
	}

	if (HAS_FLAG(type, LS_SO_ASYNC)) {
#if (LS_WINDOWS)
#	ifdef FIONBIO
		if ((ioctlsocket(ctx->fd, FIONBIO, (void*)&value)) == -1) {
			ls_log_w(LS_ERRSTR_OPERATION_FAILURE": ctx invalid");
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 1);
		}
#	else
		LS_COMPILER_WARN("FIONBIO undefined; cannot put sockets in async mode");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_UNSUPPORTED, 1);
#	endif
#else
#	ifdef O_NONBLOCK
		uint32_t flags = fcntl(ctx->fd, F_GETFL, 0);

		if (value) {
			flags |= O_NONBLOCK;
		} else {
			flags &= ~O_NONBLOCK;
		}

		if (fcntl(ctx->fd, F_SETFL, flags) == -1) {
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 2);
		}
#	else
		LS_COMPILER_WARN("O_NONBLOCK undefined; cannot put sockets in async mode");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_UNSUPPORTED, 1);
#	endif
#endif

		if (value) {
			ctx->flags |= LS_SOCKET_ASYNC;
		} else {
			ctx->flags &= ~LS_SOCKET_ASYNC;
		}
	}

	/* sub-scope for setting timeouts */
	{
		const void *optptr = &value;

#if (LS_WINDOWS) // Thanks.
		int
#else
		size_t
#endif
			optsz = sizeof(value);

#if (!LS_WINDOWS)
		struct timeval opttv = { 0 };
		opttv.tv_sec = (value / 1000);
		opttv.tv_usec = ((value - (opttv.tv_sec * 1000)) * 1000);

		optptr = &opttv;
		optsz = sizeof(opttv);
#endif

		if (HAS_FLAG(type, LS_SO_TIMEOUT_R)) {
			if (setsockopt(ctx->fd, SOL_SOCKET, SO_RCVTIMEO, optptr, optsz) != 0) {
				ls_log_w(LS_ERRSTR_OPERATION_FAILURE": unable to set socket option for SO_RCVTIMEO");
				return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 3);
			}

			if (value) {
				ctx->flags |= LS_SOCKET_TIMEOUT_R;
			} else {
				ctx->flags &= ~LS_SOCKET_TIMEOUT_R;
			}
		}

		if (HAS_FLAG(type, LS_SO_TIMEOUT_W)) {
			if (setsockopt(ctx->fd, SOL_SOCKET, SO_SNDTIMEO, optptr, optsz) != 0) {
				ls_log_w(LS_ERRSTR_OPERATION_FAILURE": unable to set socket option for SO_SNDTIMEO");
				return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 4);
			}

			if (value) {
				ctx->flags |= LS_SOCKET_TIMEOUT_W;
			} else {
				ctx->flags &= ~LS_SOCKET_TIMEOUT_W;
			}
		}
	}

	return LS_RESULT_SUCCESS;
}
