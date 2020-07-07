#include <stdbool.h>
#include <em_crypto.h>

#include "aes.h"

void aes_init(void)
{
}

bool aes_encrypt(const uint8_t *input, uint8_t *output, const uint8_t *key)
{
	unsigned int len = AES_BLOCKSIZE;

	CRYPTO0->WAC = 0;

	/* Enable AES-128 mode and set key. */
	BUS_RegBitWrite(&CRYPTO0->CTRL, _CRYPTO_CTRL_AES_SHIFT,
		_CRYPTO_CTRL_AES_AES128);
	CRYPTO_BurstToCrypto(&CRYPTO0->KEYBUF, (const uint32_t *)key);

	CRYPTO0->SEQ0 = (CRYPTO_CMD_INSTR_AESENC << _CRYPTO_SEQ0_INSTR0_SHIFT | \
		CRYPTO_CMD_INSTR_DATA0TODATA1 << _CRYPTO_SEQ0_INSTR1_SHIFT);

	len /= AES_BLOCKSIZE;
	CRYPTO0->SEQCTRL = 16 << _CRYPTO_SEQCTRL_LENGTHA_SHIFT;

	while (len--) {
		/* Load data and start encryption. */
		CRYPTO_DataWrite(&CRYPTO0->DATA0, (uint32_t *)input);
		CRYPTO0->CMD = CRYPTO_CMD_SEQSTART;

		/* Fetch encrypted data. */
		CRYPTO_DataRead(&CRYPTO0->DATA1, (uint32_t *)output);

		output += 4;
		input  += 4;
	}

	return true;
}
