#include <tinycrypt/sha256.h>

#include "sha256.h"

void sha256_init(struct sha256_context *ctx)
{
	tc_sha256_init(&ctx->tc);
}

void sha256_update(struct sha256_context *ctx, const uint8_t *data,
		unsigned int length)
{
	tc_sha256_update(&ctx->tc, data, length);
}

void sha256_final(struct sha256_context *ctx, uint8_t *digest)
{
	tc_sha256_final(digest, &ctx->tc);
}
