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
#include "../core/logging/log.h"
#include <string.h>
#include <inttypes.h>
#include <errno.h>


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
#	include <arpa/inet.h>
#endif


ID("universal socket");


uint32_t
static inline get_mtu() {
	return 4096;
}

ls_sockfd_t
static LS_ATTR_INLINE validate_sockfd(ls_sockfd_t fd, ls_bool *const valid) {
#if (LS_WINDOWS)
	return ((*valid = (fd != INVALID_SOCKET)) ? fd : LS_INVALID_SOCKET);
#else
	return ((*valid = (fd != -1)) ? fd : LS_INVALID_SOCKET);
#endif
}


ls_bool
static LS_ATTR_INLINE create_socket(ls_socket_t *const LS_RESTRICT ctx, struct addrinfo *const LS_RESTRICT addr) {
	ls_bool valid = false;

	ctx->fd = validate_sockfd(
		socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol),
		&valid
	);

	return valid;
}

ls_bool
static LS_ATTR_INLINE accept_socket(ls_sockfd_t *const LS_RESTRICT fd, const ls_socket_t *const LS_RESTRICT ctx, struct sockaddr *const LS_RESTRICT saddr, socklen_t *const LS_RESTRICT saddrlen) {
	ls_bool valid = false;

	*fd = validate_sockfd(
		accept(ctx->fd, saddr, saddrlen),
		&valid
	);

	if (!valid) {
		ls_log(LS_LOG_ERROR, "Invalid file descriptor.");
	}

	return valid;
}

ls_bool
static LS_ATTR_INLINE close_socket(ls_socket_t *const ctx) {
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
ls_socket_init_ex(ls_socket_t *const LS_RESTRICT ctx, const char *LS_RESTRICT node, const uint32_t flags) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	if (!node) {
		node = "0.0.0.0";
	}

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

	uint32_t err;

#if (LS_WINDOWS)
	if (!wsaStartup) {
		if (!(err = WSAStartup(MAKEWORD(2, 2), &wsaData))) {
			wsaStartup = true;
		} else {
			return LS_RESULT_ERROR(LS_RESULT_CODE_INITIALIZATION);
		}
	}
	++num_init_sockets;
#endif

	if ((err = getaddrinfo(node, NULL, &hints, &ctx->addrinfo))) {
		ls_log(LS_LOG_ERROR, "getaddrinfo() failed with error code: %u", err);
		return LS_RESULT_ERROR(LS_RESULT_CODE_DATA);
	}

	return LS_RESULT_SUCCESS;
}

ls_result_t
ls_socket_init(ls_socket_t *const LS_RESTRICT ctx, const char *LS_RESTRICT node) {
	return ls_socket_init_ex(ctx, node, 0);
}


ls_result_t
ls_socket_clear(ls_socket_t *const ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	if (ctx->fd != LS_INVALID_SOCKET) {
		if (!close_socket(ctx)) {
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
		return LS_RESULT_ERROR(LS_RESULT_CODE_NOT_FOUND);
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


ls_result_t
ls_socket_start(ls_socket_t *const ctx, const uint16_t port) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(ctx->addrinfo, 2);
	LS_RESULT_CHECK(!port, LS_RESULT_CODE_DATA, 1);

	const uint32_t enable = 1;
	struct addrinfo *ptr;
	uint16_t nport = htons(port);
	for (ptr = ctx->addrinfo; ptr; ptr = ptr->ai_next) {
		((struct sockaddr_in*)ptr->ai_addr)->sin_port = nport;

		if (!create_socket(ctx, ptr)) {
			ls_log(LS_LOG_ERROR, "Failed to create socket.");
			continue;
		}

		if (HAS_FLAG(ctx->flags, LS_SOCKET_SERVER)) {
#if (defined(SO_REUSEADDR))
			if (HAS_FLAG(ctx->flags, LS_SOCKET_REUSEADDR)) {
				if (setsockopt(ctx->fd, SOL_SOCKET, SO_REUSEADDR, (void*)&enable, sizeof(enable)) == -1) {
					ls_log(LS_LOG_WARNING, "Failed to enable LS_SOCKET_REUSEADDR.");
				}
			}
#else
			LS_COMPILER_WARN("LS_SOCKET_REUSEADDR unavailable: SO_REUSEADDR not defined")
#endif
#if (defined(SO_REUSEPORT))
				if (HAS_FLAG(ctx->flags, LS_SOCKET_REUSEPORT)) {
					if (setsockopt(ctx->fd, SOL_SOCKET, SO_REUSEPORT, (void*)&enable, sizeof(enable)) == -1) {
						ls_log(LS_LOG_WARNING, "Failed to enable LS_SOCKET_REUSEPORT.");
					}
				}
#else
				LS_COMPILER_WARN("LS_SOCKET_REUSEPORT unavailable: SO_REUSEPORT not defined");
#endif
			if (bind(ctx->fd, ptr->ai_addr,
#if (LS_WINDOWS) // Thanks.
					 (int)
#endif
					 ptr->ai_addrlen) == 0) {
				if (listen(ctx->fd, SOMAXCONN) == 0) {
					break;
				} else {
					ls_log(LS_LOG_WARNING, "listen()");
				}
			} else {
				ls_log(LS_LOG_WARNING, "bind()");
			}
		} else {
			if (connect(ctx->fd, ptr->ai_addr,
#if (LS_WINDOWS) // Thanks.
						(int)
#endif
						ptr->ai_addrlen) == 0) {
				break;
			} else {
				ls_log(LS_LOG_WARNING, "connect()");
			}
		}

		close_socket(ctx);
	}

	ctx->selected = ptr;

	if (!ctx->selected) {
		ctx->fd = LS_INVALID_SOCKET;
		return LS_RESULT_ERROR(LS_RESULT_CODE_INITIALIZATION);
	}

	ctx->flags &= ~LS_SOCKET_EXITED;

	if (HAS_FLAG(ctx->flags, LS_SOCKET_ASYNC)) {
		ls_socket_set_option(ctx, LS_SO_ASYNC, 1);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_socket_stop_ex(ls_socket_t *const ctx, const ls_bool force, const uint_fast16_t timeout) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_SOCKFD(ctx->fd, 1);

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
					return LS_RESULT_ERROR(LS_RESULT_CODE_TIMEOUT);
				} else {
					ls_log(LS_LOG_WARNING, "timed out, forcing shutdown");
				}
			}
		} else {
			if (!force) {
				return LS_RESULT_ERROR(LS_RESULT_CODE_CHECK);
			} else {
				ls_log(LS_LOG_WARNING, "check failed, forcing shutdown");
			}
		}
	}
#undef CHECK

#if (LS_WINDOWS)
#	define SHUTDOWN_FLAGS					SD_BOTH
#else
#	define SHUTDOWN_FLAGS					SHUT_RDWR
#endif
	if (shutdown(ctx->fd, SHUTDOWN_FLAGS) != 0) {
		if (!force) {
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_CLOSE, 1);
		} else {
			ls_log(LS_LOG_WARNING, "shutdown failed, forcing close");
		}
	}
#undef SHUTDOWN_FLAGS

	return (close_socket(ctx) ? LS_RESULT_SUCCESS : LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_CLOSE, 2));
}

ls_result_t
ls_socket_stop(ls_socket_t *const ctx, const ls_bool force) {
	return ls_socket_stop_ex(ctx, force, 0);
}


ls_result_t
ls_socket_fromfd(ls_socket_t *const ctx, const ls_sockfd_t fd, const uint32_t flags) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_SOCKFD(fd, 1);

	memset(ctx, 0, sizeof(*ctx));

	ctx->fd = fd;
	ctx->flags = flags;
	ctx->mtu = get_mtu();

	return LS_RESULT_SUCCESS;
}

ls_sockfd_t
ls_socket_acceptfd(const ls_socket_t *const LS_RESTRICT ctx, struct sockaddr *const LS_RESTRICT saddr, socklen_t *const LS_RESTRICT saddrlen) {
	if (!ctx || ctx->fd == LS_INVALID_SOCKET) {
		return LS_INVALID_SOCKET;
	}

	ls_sockfd_t fd;

	if (!accept_socket(&fd, ctx, saddr, saddrlen)) {
		ls_log(LS_LOG_WARNING, "unable to accept socket");
		return LS_INVALID_SOCKET;
	}

	return fd;
}

ls_result_t
ls_socket_accept(ls_socket_t *const LS_RESTRICT out, const ls_socket_t *const LS_RESTRICT ctx, struct sockaddr *const LS_RESTRICT saddr, socklen_t *const LS_RESTRICT saddrlen) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_NULL(out, 2);
	LS_RESULT_CHECK_SOCKFD(ctx->fd, 1);

	ls_result_t res;
	if (!(res = ls_socket_fromfd(out, ls_socket_acceptfd(ctx, saddr, saddrlen), LS_SOCKET_ACCEPTED)).success) {
		return LS_RESULT_INHERITED(res, false);
	}

#if (LS_WINDOWS)
	++num_init_sockets;
#endif

	if (HAS_FLAG(ctx->flags, LS_SOCKET_ASYNC_CHILDREN)) {
		return ls_socket_set_option(out, LS_SO_ASYNC, 1);
	}

	return LS_RESULT_SUCCESS;
}


ssize_t
static inline safe_send(const ls_socket_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT in, const uint32_t size) {
	ssize_t sent;
	while ((sent = send(ctx->fd, in, size, 0)) == 0) {
		;
	}
	return sent;
}


ls_result_t
ls_socket_write(size_t *const LS_RESTRICT out_size, const ls_socket_t *const LS_RESTRICT ctx, const void *const LS_RESTRICT in, size_t size) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_SOCKFD(ctx->fd, 1);
	LS_RESULT_CHECK_NULL(in, 2);
	LS_RESULT_CHECK_SIZE(size, 1);

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
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_WRITE, 2);
		}
	}

	if (out_size) {
		*out_size = LS_PTR_DIFF(ptr, in);
	}

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_socket_write_str(size_t *const LS_RESTRICT out_size, const ls_socket_t *const LS_RESTRICT ctx, const char *const LS_RESTRICT str) {
	return ls_socket_write(out_size, ctx, str, (str ? strlen(str) : 0));
}


ls_result_t
ls_socket_read(size_t *const LS_RESTRICT out_size, const ls_socket_t *const LS_RESTRICT ctx, void *const LS_RESTRICT out, const size_t size) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_SOCKFD(ctx->fd, 1);
	LS_RESULT_CHECK_NULL(out, 2);
	LS_RESULT_CHECK_SIZE(size, 1);

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

	if (out_size) {
		*out_size = 0;
	}

	if (HAS_FLAG(ctx->flags, LS_SOCKET_ASYNC)) {
		if (errno == EAGAIN || errno == EWOULDBLOCK) {
			return LS_RESULT_SUCCESS_CODE(LS_RESULT_CODE_EARLY_EXIT);
		}
	}

	return LS_RESULT_ERROR(LS_RESULT_CODE_READ);
}


ls_result_t
ls_socket_set_option(ls_socket_t *const ctx, const enum ls_socket_option_type type, const uint32_t value) {
	LS_RESULT_CHECK_NULL(ctx, 1);
	LS_RESULT_CHECK_SOCKFD(ctx->fd, 1);

	if (!type) {
		return LS_RESULT_ERROR(LS_RESULT_CODE_TYPE);
	}

	if (HAS_FLAG(type, LS_SO_ASYNC)) {
#if (LS_WINDOWS)
#	if (defined(FIONBIO))
		if ((ioctlsocket(ctx->fd, FIONBIO, (void*)&value)) == -1) {
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_FUNCTION, 1);
		}
#	else
		LS_COMPILER_WARN("FIONBIO not defined; cannot put sockets in async mode");
		return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_UNSUPPORTED, 1);
#	endif
#else
#	if (defined(O_NONBLOCK))
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
		LS_COMPILER_WARN("O_NONBLOCK not defined; cannot put sockets in async mode");
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
