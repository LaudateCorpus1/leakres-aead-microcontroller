#include <stm32f2xx_hal.h>
#include <stm32f2xx_hal_cryp.h>

#include "aes.h"

static CRYP_HandleTypeDef hcryp;

void aes_init(void)
{
	hcryp.Instance = CRYP;
	hcryp.Init.DataType = CRYP_DATATYPE_8B;
	hcryp.Init.KeySize = CRYP_KEYSIZE_128B;

	if (HAL_CRYP_Init(&hcryp) != HAL_OK) {
		while (true);
	}

	hcryp.Instance->CR |= (uint32_t)(CRYP_CR_ALGOMODE_AES_ECB);
	hcryp.Instance->CR |= CRYP_CR_FFLUSH;
	hcryp.Instance->CR |= CRYP_CR_CRYPEN;
}

bool aes_encrypt(const uint8_t *input, uint8_t *output, const uint8_t *key)
{
	/* Set the 128-bit key. */
	hcryp.Instance->K2LR = __REV(*(uint32_t*)(key));
	hcryp.Instance->K2RR = __REV(*(uint32_t*)(key + 4U));
	hcryp.Instance->K3LR = __REV(*(uint32_t*)(key + 8U));
	hcryp.Instance->K3RR = __REV(*(uint32_t*)(key + 12U));

	/* Write the plaintext to the accelerator. */
	hcryp.Instance->DIN = *(uint32_t*)(input);
	hcryp.Instance->DIN = *(uint32_t*)(input + 4U);
	hcryp.Instance->DIN = *(uint32_t*)(input + 8U);
	hcryp.Instance->DIN = *(uint32_t*)(input + 12U);

	/* Wait until the encryption is done. */
	while (HAL_IS_BIT_CLR(hcryp.Instance->SR, CRYP_FLAG_OFNE)) {
	}

	/* Read back the ciphertext. */
	*(uint32_t*)(output) = hcryp.Instance->DOUT;
	*(uint32_t*)(output + 4U) = hcryp.Instance->DOUT;
	*(uint32_t*)(output + 8U) = hcryp.Instance->DOUT;
	*(uint32_t*)(output + 12U) = hcryp.Instance->DOUT;

	return true;
}
