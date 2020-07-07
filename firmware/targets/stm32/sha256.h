#ifndef SHA256_H

#include <tinycrypt/sha256.h>

#define SHA256_OUTPUT_SIZE	32

struct sha256_context {
	struct tc_sha256_state_struct tc;
};

void sha256_init(struct sha256_context *ctx);
void sha256_update(struct sha256_context *ctx, const uint8_t *data,
		unsigned int length);
void sha256_final(struct sha256_context *ctx, uint8_t *digest);

#endif /* SHA256_H */
