#ifndef SHA256_H

#include <stdint.h>
#include <em_crypto.h>

#define SHA256_OUTPUT_SIZE	32

struct sha256_context {
	CRYPTO_TypeDef *crypto;
	uint8_t buffer[64];
	unsigned int buffer_length;
	uint64_t length;
};

void sha256_init(struct sha256_context *ctx);
void sha256_update(struct sha256_context *ctx, const uint8_t *data,
		unsigned int length);
void sha256_final(struct sha256_context *ctx, uint8_t *digest);

#endif /* SHA256_H */
