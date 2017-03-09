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


#if ((defined(LS_SHA2_224) || defined(LS_SHA2_256)) && !defined(LS_SHA2_32))
#	define LS_SHA2_32
#elif ((defined(LS_SHA2_384) || defined(LS_SHA2_512)) && !defined(LS_SHA2_64))
#	define LS_SHA2_64
#endif


#ifdef BITS
#undef BITS
#endif

#ifdef SHA2_CTX
#undef SHA2_CTX
#endif

#ifdef SHA2_INIT
#undef SHA2_INIT
#endif

#if (defined(LS_SHA2_224))

#elif (defined(LS_SHA2_256))
#define SHA2_CTX							struct ls_sha2_32
#define SHA2_INIT							ls_sha2_256_init
#elif (defined(LS_SHA2_384))

#elif (defined(LS_SHA2_512))
#define SHA2_CTX							struct ls_sha2_64
#define SHA2_INIT							ls_sha2_512_init
#endif


#if (defined(LS_SHA2_32) && !defined(LS_SHA2_32_C))
#	define LS_SHA2_32_C

struct ls_sha2_32 {
	uint32_t h[8];
};

static const uint32_t constants32[] = {
	0x428A2F98U, 0x71374491U, 0xB5C0FBCFU, 0xE9B5DBA5U,
	0x3956C25BU, 0x59F111F1U, 0x923F82A4U, 0xAB1C5ED5U,
	0xD807AA98U, 0x12835B01U, 0x243185BEU, 0x550C7DC3U,
	0x72BE5D74U, 0x80DEB1FEU, 0x9BDC06A7U, 0xC19BF174U,
	0xE49B69C1U, 0xEFBE4786U, 0x0FC19DC6U, 0x240CA1CCU,
	0x2DE92C6FU, 0x4A7484AAU, 0x5CB0A9DCU, 0x76F988DAU,
	0x983E5152U, 0xA831C66DU, 0xB00327C8U, 0xBF597FC7U,
	0xC6E00BF3U, 0xD5A79147U, 0x06CA6351U, 0x14292967U,
	0x27B70A85U, 0x2E1B2138U, 0x4D2C6DFCU, 0x53380D13U,
	0x650A7354U, 0x766A0ABBU, 0x81C2C92EU, 0x92722C85U,
	0xA2BFE8A1U, 0xA81A664BU, 0xC24B8B70U, 0xC76C51A3U,
	0xD192E819U, 0xD6990624U, 0xF40E3585U, 0x106AA070U,
	0x19A4C116U, 0x1E376C08U, 0x2748774CU, 0x34B0BCB5U,
	0x391C0CB3U, 0x4ED8AA4AU, 0x5B9CCA4FU, 0x682E6FF3U,
	0x748F82EEU, 0x78A5636FU, 0x84C87814U, 0x8CC70208U,
	0x90BEFFFAU, 0xA4506CEBU, 0xBEF9A3F7U, 0xC67178F2U
};
#elif (defined(LS_SHA2_64) && !defined(LS_SHA2_64_C))
#	define LS_SHA2_64_C

struct ls_sha2_64 {
	uint64_t h[8];
};

static const uint64_t constants64[] = {
	0x428A2F98D728AE22U, 0x7137449123EF65CDU, 0xB5C0FBCFEC4D3B2FU, 0xE9B5DBA58189DBBCU,
	0x3956C25BF348B538U, 0x59F111F1B605D019U, 0x923F82A4AF194F9BU, 0xAB1C5ED5DA6D8118U,
	0xD807AA98A3030242U, 0x12835B0145706FBEU, 0x243185BE4EE4B28CU, 0x550C7DC3D5FFB4E2U,
	0x72BE5D74F27B896FU, 0x80DEB1FE3B1696B1U, 0x9BDC06A725C71235U, 0xC19BF174CF692694U,
	0xE49B69C19EF14AD2U, 0xEFBE4786384F25E3U, 0x0FC19DC68B8CD5B5U, 0x240CA1CC77AC9C65U,
	0x2DE92C6F592B0275U, 0x4A7484AA6EA6E483U, 0x5CB0A9DCBD41FBD4U, 0x76F988DA831153B5U,
	0x983E5152EE66DFABU, 0xA831C66D2DB43210U, 0xB00327C898FB213FU, 0xBF597FC7BEEF0EE4U,
	0xC6E00BF33DA88FC2U, 0xD5A79147930AA725U, 0x06CA6351E003826FU, 0x142929670A0E6E70U,
	0x27B70A8546D22FFCU, 0x2E1B21385C26C926U, 0x4D2C6DFC5AC42AEDU, 0x53380D139D95B3DFU,
	0x650A73548BAF63DEU, 0x766A0ABB3C77B2A8U, 0x81C2C92E47EDAEE6U, 0x92722C851482353BU,
	0xA2BFE8A14CF10364U, 0xA81A664BBC423001U, 0xC24B8B70D0F89791U, 0xC76C51A30654BE30U,
	0xD192E819D6EF5218U, 0xD69906245565A910U, 0xF40E35855771202AU, 0x106AA07032BBD1B8U,
	0x19A4C116B8D2D0C8U, 0x1E376C085141AB53U, 0x2748774CDF8EEB99U, 0x34B0BCB5E19B48A8U,
	0x391C0CB3C5C95A63U, 0x4ED8AA4AE3418ACBU, 0x5B9CCA4F7763E373U, 0x682E6FF3D6B2B8A3U,
	0x748F82EE5DEFB2FCU, 0x78A5636F43172F60U, 0x84C87814A1F0AB72U, 0x8CC702081A6439ECU,
	0x90BEFFFA23631E28U, 0xA4506CEBDE82BDE9U, 0xBEF9A3F7B2C67915U, 0xC67178F2E372532BU,
	0xCA273ECEEA26619CU, 0xD186B8C721C0C207U, 0xEADA7DD6CDE0EB1EU, 0xF57D4F7FEE6ED178U,
	0x06F067AA72176FBAU, 0x0A637DC5A2C898A6U, 0x113F9804BEF90DAEU, 0x1B710B35131C471BU,
	0x28DB77F523047D84U, 0x32CAAB7B40C72493U, 0x3C9EBE0A15C9BEBCU, 0x431D67C49C100D4CU,
	0x4CC5D4BECB3E42B6U, 0x597F299CFC657E2AU, 0x5FCB6FAB3AD6FAECU, 0x6C44198C4A475817U
};
#endif


/*
#if (defined(LS_SHA2_224))

#elif (defined(LS_SHA2_256))

#elif (defined(LS_SHA2_384))

#elif (defined(LS_SHA2_512))

#endif
*/


#ifdef SHA2_CTX
ls_result_t
SHA2_INIT(SHA2_CTX *ctx) {
	LS_RESULT_CHECK_NULL(ctx, 1);

	memset(ctx, 0, sizeof(*ctx));

#if (defined(LS_SHA2_224))
	ctx->h[0] = 0xC1059ED8U;
	ctx->h[1] = 0x367CD507U;
	ctx->h[2] = 0x3070DD17U;
	ctx->h[3] = 0xF70E5939U;
	ctx->h[4] = 0xFFC00B31U;
	ctx->h[5] = 0x68581511U;
	ctx->h[6] = 0x64F98FA7U;
	ctx->h[7] = 0xBEFA4FA4U;
#elif (defined(LS_SHA2_256))
	ctx->h[0] = 0x6A09E667U;
	ctx->h[1] = 0xBB67AE85U;
	ctx->h[2] = 0x3C6EF372U;
	ctx->h[3] = 0xA54FF53AU;
	ctx->h[4] = 0x510E527FU;
	ctx->h[5] = 0x9B05688CU;
	ctx->h[6] = 0x1F83D9ABU;
	ctx->h[7] = 0x5BE0CD19U;
#elif (defined(LS_SHA2_384))
	ctx->h[0] = 0xCBBB9D5DC1059ED8U;
	ctx->h[1] = 0x629A292A367CD507U;
	ctx->h[2] = 0x9159015A3070DD17U;
	ctx->h[3] = 0x152FECD8F70E5939U;
	ctx->h[4] = 0x67332667FFC00B31U;
	ctx->h[5] = 0x8EB44A8768581511U;
	ctx->h[6] = 0xDB0C2E0D64F98FA7U;
	ctx->h[7] = 0X47B5481DBEFA4FA4U;
#elif (defined(LS_SHA2_512))
	ctx->h[0] = 0x6A09E667F3BCC908U;
	ctx->h[1] = 0xBB67AE8584CAA73BU;
	ctx->h[2] = 0x3C6EF372FE94F82BU;
	ctx->h[3] = 0xA54FF53A5F1D36F1U;
	ctx->h[4] = 0x510E527FADE682D1U;
	ctx->h[5] = 0x9B05688C2B3E6C1FU;
	ctx->h[6] = 0x1F83D9ABFB41BD6BU;
	ctx->h[7] = 0x5BE0CD19137E2179U;
#endif

	return LS_RESULT_SUCCESS;
}
#endif