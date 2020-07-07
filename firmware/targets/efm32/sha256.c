#include <string.h>
#include <em_crypto.h>

#include "sha256.h"

#define SHA256_BLOCK_SIZE	64

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

static const uint32_t sha256_init_values[] = {
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

void sha256_init(struct sha256_context *ctx)
{
	ctx->crypto = CRYPTO;
	ctx->buffer_length = 0;
	ctx->length = 0;

	/* Initialize crypto module for SHA-256. */
	ctx->crypto->CTRL = CRYPTO_CTRL_SHA_SHA2;
	ctx->crypto->SEQCTRL = 0;
	ctx->crypto->SEQCTRLB = 0;
	CRYPTO_ResultWidthSet(ctx->crypto, cryptoResult256Bits);

	CRYPTO_DDataWrite(&ctx->crypto->DDATA1, sha256_init_values);

	/* SHA preparations according to table 30.7 in the EFM32PG RM. */
	CRYPTO_EXECUTE_2(ctx->crypto, CRYPTO_CMD_INSTR_DDATA1TODDATA0,
		CRYPTO_CMD_INSTR_SELDDATA0DDATA1);
}

void sha256_update(struct sha256_context *ctx, const uint8_t *data,
		unsigned int length)
{
	uint8_t block[SHA256_BLOCK_SIZE];

	ctx->length += length;

	if (ctx->buffer_length + length < SHA256_BLOCK_SIZE) {
		memcpy(ctx->buffer + ctx->buffer_length, data, length);
		ctx->buffer_length += length;
		return;
	}

	if (ctx->buffer_length > 0) {
		unsigned int tmp = MIN(SHA256_BLOCK_SIZE - ctx->buffer_length,
			length);

		memcpy(block, ctx->buffer, ctx->buffer_length);
		memcpy(block + ctx->buffer_length, data, tmp);

		CRYPTO_QDataWrite(&ctx->crypto->QDATA1BIG, (uint32_t *)block);

		CRYPTO_EXECUTE_3(ctx->crypto, CRYPTO_CMD_INSTR_SHA,
			CRYPTO_CMD_INSTR_MADD32,
			CRYPTO_CMD_INSTR_DDATA0TODDATA1);

		length -= tmp;
		data += tmp;
	}

	while (length >= SHA256_BLOCK_SIZE) {
		CRYPTO_QDataWrite(&ctx->crypto->QDATA1BIG, (uint32_t *)data);

		CRYPTO_EXECUTE_3(ctx->crypto, CRYPTO_CMD_INSTR_SHA,
			CRYPTO_CMD_INSTR_MADD32,
			CRYPTO_CMD_INSTR_DDATA0TODDATA1);

		length -= SHA256_BLOCK_SIZE;
		data += SHA256_BLOCK_SIZE;
	}

	/* Copy remaining data. */
	memcpy(ctx->buffer, data, length);
	ctx->buffer_length = length;
}

void sha256_final(struct sha256_context *ctx, uint8_t *digest)
{
	ctx->buffer[ctx->buffer_length++] = 0x80;

	/* Not enough space left for the data size. */
	if (ctx->buffer_length > 56) {
		while (ctx->buffer_length < 64)
			ctx->buffer[ctx->buffer_length++] = 0;

		CRYPTO_QDataWrite(&ctx->crypto->QDATA1BIG,
			(uint32_t *)ctx->buffer);

		CRYPTO_EXECUTE_3(ctx->crypto, CRYPTO_CMD_INSTR_SHA,
			CRYPTO_CMD_INSTR_MADD32,
			CRYPTO_CMD_INSTR_DDATA0TODDATA1);

		ctx->buffer_length = 0;
	}

	while (ctx->buffer_length < 56)
		ctx->buffer[ctx->buffer_length++] = 0;

	uint64_t data_length = ctx->length;

	/* Append the data length in bits and big endian byte order. */
	ctx->buffer[ctx->buffer_length++] = (uint8_t)(data_length >> 53);
	ctx->buffer[ctx->buffer_length++] = (uint8_t)(data_length >> 45);
	ctx->buffer[ctx->buffer_length++] = (uint8_t)(data_length >> 37);
	ctx->buffer[ctx->buffer_length++] = (uint8_t)(data_length >> 29);
	ctx->buffer[ctx->buffer_length++] = (uint8_t)(data_length >> 21);
	ctx->buffer[ctx->buffer_length++] = (uint8_t)(data_length >> 13);
	ctx->buffer[ctx->buffer_length++] = (uint8_t)(data_length >> 5);
	ctx->buffer[ctx->buffer_length++] = (uint8_t)(data_length << 3);

	CRYPTO_QDataWrite(&ctx->crypto->QDATA1BIG, (uint32_t *)ctx->buffer);

	CRYPTO_EXECUTE_3(ctx->crypto, CRYPTO_CMD_INSTR_SHA,
		CRYPTO_CMD_INSTR_MADD32,
		CRYPTO_CMD_INSTR_DDATA0TODDATA1);

	CRYPTO_DDataRead(&ctx->crypto->DDATA0BIG, (uint32_t *)digest);
}
