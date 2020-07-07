#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "device.h"
#include "aes.h"
#include "lraead.h"
#include "lraead_testdata.h"

static uint8_t lraead_buffer[2 * 1024];

int main(void)
{
	device_init();
	aes_init();

	/**
	 * Copy ciphertext from flash to SRAM to enable in-place decryption.
	 */
	memcpy(lraead_buffer, ctxt, input.msg_len);
	input.msg = lraead_buffer;

	lr_result result = lraead(&input, &config, lraead_buffer);

	if (result != LR_RES_SUCCESS) {
		while (true);
	}

	device_led_on();

	while (true) {
	}
}
