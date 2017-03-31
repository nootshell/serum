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

#define FILE_PATH							"networking/packet-decoder.c"

#include "./packet-decoder.h"
#include <string.h>


ID("packet decoder");


ls_result_t
ls_packet_decoder_init_ex(ls_packet_decoder_t *decoder, void(*callback)(struct ls_packet_decoder *decoder, ls_packet_t *packet), void *tag, uint32_t flags) {
	LS_RESULT_CHECK_NULL(decoder, 1);

	memset(decoder, 0, sizeof(*decoder));
	decoder->callback = callback;
	decoder->tag = tag;
	decoder->flags = flags;
	decoder->__state = LS_DECODE_STATE_HEAD;

	return LS_RESULT_SUCCESS;
}


ls_result_t
ls_packet_decoder_init(ls_packet_decoder_t *decoder) {
	return ls_packet_decoder_init_ex(decoder, NULL, NULL, 0);
}


ls_result_t
ls_packet_decoder_clear(ls_packet_decoder_t *decoder) {
	LS_RESULT_CHECK_NULL(decoder, 1);

	ls_packet_clear_ex(&decoder->packet, true, true);
	memset(decoder, 0, sizeof(*decoder));

	return LS_RESULT_SUCCESS;
}


void
static LS_ATTR_INLINE __dispatch(ls_packet_decoder_t *decoder) {
	decoder->__state = LS_DECODE_STATE_HEAD;
	++(decoder->decode_count);
	if (decoder->callback) {
		decoder->callback(decoder, &decoder->packet);
	}
	ls_packet_clear_ex(&decoder->packet, true, true);
}

void
static LS_ATTR_INLINE __alloc_header(ls_packet_decoder_t *decoder, uint8_t size) {
	decoder->__header = &decoder->packet.headers[decoder->__sub_index];
	decoder->__header->size = size;
	decoder->__header->value = malloc(size);
}

size_t
static LS_ATTR_INLINE __valuecpy(ls_packet_decoder_t *decoder, void *out, size_t total_size, void *in, size_t start_index, size_t buffer_size) {
	size_t req;
	if ((start_index + (req = (total_size - decoder->__index))) >= buffer_size) {
		req = (buffer_size - start_index);
	}

	memcpy(
		(((uint8_t*)out) + decoder->__index),
		(((uint8_t*)in) + start_index),
		req
	);

	decoder->__index += req;

	return (req - 1);
}

ls_result_t
ls_packet_decode(ls_packet_decoder_t *decoder, void *in, size_t size) {
	LS_RESULT_CHECK_NULL(decoder, 1);
	LS_RESULT_CHECK_NULL(in, 2);
	LS_RESULT_CHECK_SIZE(size, 1);

	uint8_t byte;

	size_t i;
	for (i = 0; i < size; ++i) {
		byte = ((uint8_t*)in)[i];
		if (decoder->__state == LS_DECODE_STATE_HEAD) {
			decoder->packet.command = ((byte >> 4) & 0x0F);
			decoder->packet.header_count = ((byte) & 0x0F);
			decoder->__state = LS_DECODE_STATE_FLAGS;
		} else if (decoder->__state == LS_DECODE_STATE_FLAGS) {
			decoder->packet.flags = byte;
			if (decoder->packet.header_count) {
				decoder->__sub_index = 0;
				decoder->packet.headers = malloc(sizeof(*decoder->packet.headers) * decoder->packet.header_count);
				decoder->__state = LS_DECODE_STATE_HEADER_SIZE;
			} else {
				if (HAS_FLAG(decoder->packet.flags, LS_PACKET_PAYLOAD)) {
					decoder->__sub_index = 0;
					decoder->__state = LS_DECODE_STATE_PAYLOAD_SIZE;
				} else {
					__dispatch(decoder);
				}
			}
		} else  if (decoder->__state == LS_DECODE_STATE_HEADER_SIZE) {
			__alloc_header(decoder, byte);
			decoder->__index = 0;
			decoder->__state = LS_DECODE_STATE_HEADER_VALUE;
		} else if (decoder->__state == LS_DECODE_STATE_HEADER_VALUE) {
			i += __valuecpy(decoder, decoder->__header->value, decoder->__header->size, in, i, size);

			if (decoder->__index >= decoder->__header->size) {
				if (++decoder->__sub_index >= decoder->packet.header_count) {
					if (HAS_FLAG(decoder->packet.flags, LS_PACKET_PAYLOAD)) {
						decoder->__sub_index = 0;
						decoder->__state = LS_DECODE_STATE_PAYLOAD_SIZE;
					} else {
						__dispatch(decoder);
					}
				} else {
					decoder->__state = LS_DECODE_STATE_HEADER_SIZE;
				}
			}
		} else  if (decoder->__state == LS_DECODE_STATE_PAYLOAD_SIZE) {
			if (ls_varsize_get_value_stateless(&decoder->__varsize_buffer, byte, &decoder->__sub_index)) {
				decoder->__sub_index = 0;
				decoder->packet.payload_size = ((uint32_t)decoder->__varsize_buffer);
				decoder->packet.payload = malloc(decoder->packet.payload_size);
				decoder->__index = 0;
				decoder->__state = LS_DECODE_STATE_PAYLOAD_VALUE;
			}
		} else if (decoder->__state == LS_DECODE_STATE_PAYLOAD_VALUE) {
			i += __valuecpy(decoder, decoder->packet.payload, decoder->packet.payload_size, in, i, size);

			if (decoder->__index >= decoder->packet.payload_size) {
				__dispatch(decoder);
			}
		}
	}

	return LS_RESULT_SUCCESS;
}
