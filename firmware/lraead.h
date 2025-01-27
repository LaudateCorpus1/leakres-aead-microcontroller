#ifndef LRAEAD_H
#define LRAEAD_H

#include "lrprf.h"

typedef enum {
	LR_AEAD_ENCRYPT,
	LR_AEAD_DECRYPT,
} lr_aead_mode;

struct aead_input {
        uint8_t iv[16];
        /* if mode is encrypt: plaintext, if mode is decrypt: ciphertext */
        uint8_t *msg;
        size_t msg_len;
        uint8_t *adata;
        size_t adata_len;
        uint8_t tag[16];
};

struct aead_config {
        const uint8_t *enckey;
        size_t enckey_len;
        const uint8_t *mackey;
        size_t mackey_len;
        /* data complexity */
        lr_dc dc;
	/* mode, encrypt not required on uCs */
	lr_aead_mode mode;
};

/*
 * returns LR_RES_VERIFY_SUCCESS and msg_len bytes of plaintext in output if tag is valid, LR_RES_VERIFY_FAIL and no output otherwise
 * output can point to aead_input->msg for in place calculation
 */
lr_result lraead(struct aead_input *aead_input,
		const struct aead_config *aead_config, uint8_t *output);

#endif /* LRAEAD_H */
