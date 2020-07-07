#include <stdint.h>
#include <string.h>

#include "aes.h"
#include "lrprf.h"

static void gen_plaintext(const lr_dc dc, uint8_t iv_bitval, uint8_t *ptxt)
{
	unsigned int bit_size = dc;
	uint8_t b = 0;

	for (unsigned int i = 0; i < 8 / bit_size; i++) {
		b |= iv_bitval << (i * bit_size);
	}

	for (unsigned int i = 0; i < 16; i++){
		ptxt[i] = b;
	}
}

lr_result lrprf(const uint8_t *data, size_t data_len, const uint8_t *key,
		size_t key_len, const lr_dc dc, uint8_t *output)
{
	uint8_t plaintext[16];
	const unsigned int bit_size = dc;
	const unsigned int mask = (2 << (bit_size - 1)) - 1;

	if (!data || !key || !output)
		return LR_RES_INVALID_ARG;

	(void)key_len;

	memcpy(output, key, LRPRF_OUTPUT_SIZE);

	for (unsigned int b = 0; b < data_len; b++) {
		for (unsigned int i = 0; i < 8; i += bit_size) {
			const unsigned int shift = 8 - i - bit_size;
			const uint8_t tmp = data[b] & (mask << shift);
			const uint8_t p = tmp >> shift;

			gen_plaintext(dc, p, plaintext);

			if (!aes_encrypt(plaintext, output, output))
				return LR_RES_FAIL;
		}
	}

	/* Whitening step, alwayss encrypt zeros. */
	memset(plaintext, 0, sizeof(plaintext));
	if (!aes_encrypt(plaintext, output, output)) {
		return LR_RES_FAIL;
	}

	return LR_RES_SUCCESS;
}
