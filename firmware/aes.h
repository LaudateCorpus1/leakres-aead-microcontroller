#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stdbool.h>

#define AES_BLOCKSIZE	16

void aes_init(void);

bool aes_encrypt(const uint8_t *input, uint8_t *output, const uint8_t *key);

#endif /* AES_H */
