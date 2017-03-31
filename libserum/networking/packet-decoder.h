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

#ifndef __LS_NETWORKING_PACKET_DECODER_H
#define __LS_NETWORKING_PACKET_DECODER_H


#include "../core/stdincl.h"
#include "../core/varsize.h"
#include "./packet.h"


typedef enum ls_packet_decoder_state {
	LS_DECODE_STATE_UNKNOWN					= 0U,
	LS_DECODE_STATE_HEAD					= 1U,
	LS_DECODE_STATE_FLAGS					= 2U,
	LS_DECODE_STATE_HEADER_SIZE				= 3U,
	LS_DECODE_STATE_HEADER_VALUE			= 4U,
	LS_DECODE_STATE_PAYLOAD_SIZE			= 5U,
	LS_DECODE_STATE_PAYLOAD_VALUE			= 6U
} ls_packet_decoder_state_t;

typedef struct ls_packet_decoder {
	ls_vs_value_t __varsize_buffer;

	void(*callback)(struct ls_packet_decoder *decoder, ls_packet_t *packet);
	void *tag;

	ls_packet_t packet;
	ls_packet_header_t *__header;

	size_t __index;
	uint_fast16_t __sub_index;
	uint16_t __state;

	uint16_t flags;
	uint32_t decode_count;

} ls_packet_decoder_t;


#ifdef __cplusplus
extern "C" {
#endif

	LSAPI ls_result_t ls_packet_decoder_init_ex(ls_packet_decoder_t *decoder, void(*callback)(struct ls_packet_decoder *decoder, ls_packet_t *packet), void *tag, uint32_t flags);
	LSAPI ls_result_t ls_packet_decoder_init(ls_packet_decoder_t *decoder);
	LSAPI ls_result_t ls_packet_decoder_clear(ls_packet_decoder_t *decoder);

	LSAPI ls_result_t ls_packet_decode(ls_packet_decoder_t *decoder, void *in, size_t size);

#ifdef __cplusplus
}
#endif


#endif
