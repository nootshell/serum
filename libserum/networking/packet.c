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

#define FILE_PATH							"networking/packet.c"

#include "./packet.h"
#include "../core/memory.h"
#include "../core/varsize.h"
#include "../core/logging/log.h"
#include <string.h>


ID("built-in packet structure");


ls_result_t
ls_packet_init(ls_packet_t *const packet, const uint8_t command, const uint8_t flags) {
	LS_RESULT_CHECK_NULL(packet, 1);

	packet->headers = NULL;
	packet->payload = NULL;
	packet->payload_size = 0;
	packet->command = (command & 0x0F);
	packet->header_count = 0;
	packet->flags = flags;
	packet->__h_alloc_sz = 0;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_packet_clear_ex(ls_packet_t *const packet, const ls_bool free_headers, const ls_bool free_payload) {
	LS_RESULT_CHECK_NULL(packet, 1);

	if (packet->headers) {
		if (free_headers) {
			ls_packet_header_t *header;
			ls_nword_t i;
			for (i = packet->header_count; i--;) {
				header = &packet->headers[i];
				if (header->value) {
					free(header->value);
				}
			}
		}
		free(packet->headers);
	}

	if (free_payload && packet->payload) {
		free(packet->payload);
	}

	packet->headers = NULL;
	packet->payload = NULL;
	packet->payload_size = 0;
	packet->command = 0;
	packet->header_count = 0;
	packet->flags = 0;
	packet->__h_alloc_sz = 0;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_packet_clear(ls_packet_t *const packet) {
	return ls_packet_clear_ex(packet, false, false);
}


ls_result_t
ls_packet_add_header(ls_packet_t *const LS_RESTRICT packet, const uint8_t size, const void *const LS_RESTRICT value) {
	LS_RESULT_CHECK_NULL(packet, 1);
	LS_RESULT_CHECK_SIZE(size, 1);
	LS_RESULT_CHECK_NULL(value, 2);

	if (!packet->headers) {
		packet->headers = malloc(sizeof(*packet->headers));
		packet->__h_alloc_sz = 1;

		if (!packet->headers) {
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_ALLOCATION, 1);
		}
	}

	if (packet->header_count >= packet->__h_alloc_sz) {
		void *ptr = realloc(
			packet->headers,
			(sizeof(*packet->headers) * (packet->__h_alloc_sz = ((packet->header_count) + 1)))
		);

		if (!ptr) {
			return LS_RESULT_ERROR_PARAM(LS_RESULT_CODE_ALLOCATION, 2);
		} else {
			packet->headers = ptr;
		}
	}

	ls_packet_header_t *header = &packet->headers[packet->header_count++];
	header->size = size;
	header->value = (void*)value;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_packet_set_payload(ls_packet_t *const LS_RESTRICT packet, const uint32_t size, const void *const LS_RESTRICT value) {
	LS_RESULT_CHECK_NULL(packet, 1);
	LS_RESULT_CHECK_SIZE(size, 1);
	LS_RESULT_CHECK_NULL(value, 2);

	packet->payload_size = size;
	packet->payload = (void*)value;

	if (packet->payload && packet->payload_size) {
		packet->flags |= LS_PACKET_PAYLOAD;
	} else {
		packet->flags &= ~LS_PACKET_PAYLOAD;
	}

	return LS_RESULT_SUCCESS;
}


void*
ls_packet_encode(const ls_packet_t *const LS_RESTRICT packet, size_t *const LS_RESTRICT out_size) {
	if (!packet) {
		ls_log(LS_LOG_ERROR, "packet null");
		return NULL;
	}

	if (!out_size) {
		ls_log(LS_LOG_ERROR, "out_size null");
		return NULL;
	}

	size_t size = 2, psz_size = 0;
	uint8_t psz_buffer[10];

	if (packet->header_count && packet->headers) {
		ls_packet_header_t *header;
		ls_nword_t i;
		for (i = 0; i < packet->header_count; ++i) {
			header = &packet->headers[i];
			if (header->size && header->value) {
				size += (1 + header->size);
			}
		}
	}

	if (HAS_FLAG(packet->flags, LS_PACKET_PAYLOAD) && packet->payload_size && packet->payload) {
		if (!ls_varsize_get_bytes(psz_buffer, &psz_size, packet->payload_size).success || !psz_size) {
			ls_log(LS_LOG_ERROR, "variable-size integer");
			return NULL;
		}
		size += (psz_size + packet->payload_size);
	}

	size_t i = 0;
	uint8_t *enc = malloc(*out_size = size);
	enc[i++] = (((packet->command & 0x0F) << 4) | (packet->header_count & 0x0F));
	enc[i++] = packet->flags;

	if (packet->header_count && packet->headers) {
		ls_packet_header_t *header;
		ls_nword_t j;
		for (j = 0; j < packet->header_count; ++j) {
			header = &packet->headers[j];
			if (header->size && header->value) {
				enc[i++] = (header->size);
				memcpy(enc + i, header->value, header->size);
				i += header->size;
			}
		}
	}

	if (HAS_FLAG(packet->flags, LS_PACKET_PAYLOAD)) {
		memcpy(enc + i, psz_buffer, psz_size);
		i += psz_size;

		memcpy(enc + i, packet->payload, packet->payload_size);
		i += packet->payload_size;
	}

	return enc;
}
