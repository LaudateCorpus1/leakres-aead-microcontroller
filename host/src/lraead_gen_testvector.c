#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "lraead.h"

static uint8_t keys[2][16] = {
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // k_enc
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // k_mac
};

/* print arrays in C include format */
static void printi(uint8_t* h, size_t size) {
	size_t i = 0;
	printf("\t");
	for (i = 0; i < size; i++) {
		printf("0x%02X", h[i]);
		if (i != size-1) {
			printf(", ");
			if (((i+1) % 8 == 0)) printf("\n\t");
		}
	}
}

static void print_include_header() {
	printf(
		"#ifndef LRAEAD_TESTDATA_H\n"
		"#define LRAEAD_TESTDATA_H\n"
		"#include \"lraead.h\"\n\n"
	);
}

static void print_include_footer() {
	printf("#endif /* LRAEAD_TESTDATA_H */\n");
}

static void print_keys() {
	printf("static uint8_t enckey[] = {\n");
	printi(keys[0], 16);
	printf("\n};\n\n");
	printf("static uint8_t mackey[] = {\n");
	printi(keys[1], 16);
	printf("\n};\n\n");
}

static void print_data(char *name, uint8_t *data, int data_len) {
	printf("static uint8_t %s[] = {\n", name);
	printi(data, data_len);
	printf("\n};\n\n");
}

static void print_input(struct aead_input *input) {
	/* print adata, if any */
	if (input->adata_len) {
		print_data("adata", input->adata, input->adata_len);
	}

	/* print ciphertext */
	print_data("ctxt", input->msg, input->msg_len);

	/* print input struct */
	printf("static struct aead_input input = {\n");
	/* iv */
	printf("\t{\n");
	printi(input->iv, 16);
	printf("\n\t},\n");
	printf("\tctxt,\n");
	printf("\tsizeof(ctxt),\n");
	if (input->adata_len) {
		printf("\tadata,\n");
		printf("\tsizeof(adata),\n");
	} else {
		printf("\tNULL,\n");
		printf("\t0,\n");
	}
	/* tag */
	printf("\t{\n");
	printi(input->tag, 16);
	printf("\n\t}\n");
	printf("};\n\n");

}

static void print_config(struct aead_config *conf) {
	int print_dc;
	print_dc = (int) pow(2., (float)conf->dc);

	printf(
	"static struct aead_config config = {\n"
	"	enckey,\n"
	"	sizeof(enckey),\n"
	"	mackey,\n"
	"	sizeof(mackey),\n"
	"	DC_%d,\n"
	"	%s,\n"
	"};\n"
	"\n", print_dc, conf->mode ? "LR_AEAD_DECRYPT" : "LR_AEAD_ENCRYPT");
}

int main(int argc, char **argv) {
	int rc;
	FILE* f;
	size_t adata_len;
	size_t msg_len;
	uint8_t* adata = NULL;
	uint8_t* msg = NULL;
	uint8_t* ctxtbuf = NULL;
	lr_dc dc = DC_2;
	struct aead_input aead_input;
	struct aead_config aead_config;
	lr_result lrc;

	if (argc < 5 || argc > 6) {
		fprintf(stderr, "Usage: %s adata_filename message_filename key_filename nonce_filename [data_complexity]\n", argv[0]);
		return -1;
	}

	if (argc == 6) {
		int tmp;

		if (sscanf(argv[5], "%i", &tmp) < 0) {
			fprintf(stderr, "Invalid data complexity value\n");
			return -1;
		}

		switch (tmp) {
		case 2:
			dc = DC_2;
			break;
		case 4:
			dc = DC_4;
			break;
		case 16:
			dc = DC_16;
			break;
		case 256:
			dc = DC_256;
			break;
		default:
			fprintf(stderr, "Invalid data complexity value\n");
			return -1;
		}
	}

	/* parse key */
	f = fopen(argv[3], "r");
	if (!f) {
		fprintf(stderr, "Could not open key file: %s\n", argv[3]);
		return -1;
	}

	// get key size
	fseek(f, 0, SEEK_END);
	if (ftell(f) != sizeof(keys)) {

		fprintf(stderr, "Invalid key length, expected %lu\n", sizeof(keys));
		return -1;
	}
	rewind(f);

	rc = fread(keys, 1, sizeof(keys), f);
	if (rc != sizeof(keys)) {
		fprintf(stderr, "Could not read full key, expected %lu bytes, received %d\n", sizeof(keys), rc);
		return -1;
	}
	fclose(f);

	/* parse nonce */
	f = fopen(argv[4], "r");
	if (!f) {
		fprintf(stderr, "Could not open nonce file: %s\n", argv[4]);
		return -1;
	}

	// get nonce size
	fseek(f, 0, SEEK_END);
	if (ftell(f) != sizeof(aead_input.iv)) {
		fprintf(stderr, "Invalid nonce length, expected %lu\n", sizeof(aead_input.iv));
		return -1;
	}
	rewind(f);

	rc = fread(aead_input.iv, 1, sizeof(aead_input.iv), f);
	if (rc != sizeof(aead_input.iv)) {
		fprintf(stderr, "Could not read full key, expected %lu bytes, received %d\n", sizeof(aead_input.iv), rc);
		return -1;
	}
	fclose(f);

	f = fopen(argv[2], "r");
	if (!f) {
		fprintf(stderr, "Could not open message file: %s\n", argv[2]);
		return -1;
	}
	// get input size
	fseek(f, 0, SEEK_END);
	msg_len = ftell(f);
	rewind(f);

	msg = malloc(msg_len);
	if (!msg) {
		fprintf(stderr, "malloc failed.\n");
		return -1;
	}
	ctxtbuf = malloc(msg_len);
	if (!ctxtbuf) {
		fprintf(stderr, "malloc failed.\n");
		return -1;
	}

	if (fread(msg, 1, msg_len, f) != msg_len) {
		fprintf(stderr, "Could not read full message, expected %zi bytes\n", msg_len);
		return -1;
	}
	fclose(f);

	f = fopen(argv[1], "r");
	if (!f) {
		fprintf(stderr,"Could not open adata file: %s\n", argv[1]);
		return -1;
	}
	// get input size
	fseek(f, 0, SEEK_END);
	adata_len = ftell(f);
	rewind(f);

	if (adata_len > 0) {
		adata = malloc(adata_len);
		if (!adata) {
			fprintf(stderr, "malloc failed.\n");
			return -1;
		}

		if (fread(adata, 1, adata_len, f) != adata_len) {
			fprintf(stderr, "Could not read full adata, expected %zi bytes\n", adata_len);
			return -1;
		}
		fclose(f);
	}

	/* prepare aead input */
	aead_input.msg = msg;
	aead_input.msg_len = msg_len;
	aead_input.adata = adata;
	aead_input.adata_len = adata_len;

	aead_config.enckey = keys[0];
	aead_config.enckey_len = sizeof(keys[0]);
	aead_config.mackey = keys[1];
	aead_config.mackey_len = sizeof(keys[1]);
	aead_config.dc = dc;
	aead_config.mode = LR_AEAD_ENCRYPT;


	lrc = lraead(&aead_input, &aead_config, ctxtbuf);
	if (lrc != LR_RES_VERIFY_SUCCESS) {
		fprintf(stderr, "lraead failed\n");
		return -1;
	}

	/* print decryption config struct and ciphertext */
	aead_config.mode = LR_AEAD_DECRYPT;
	aead_input.msg = ctxtbuf;

	print_include_header();
	print_keys();
	print_config(&aead_config);
	print_input(&aead_input);
	print_include_footer();

	/* selfcheck: decrypt and compare to plaintext */
	lrc = lraead(&aead_input, &aead_config, ctxtbuf);
	if (lrc != LR_RES_VERIFY_SUCCESS) {
		fprintf(stderr, "lraead failed\n");
		return -1;
	}

	if (memcmp(ctxtbuf, msg, msg_len)) {
		fprintf(stderr, "decryption selfcheck failed\n");
		return -1;
	}
	return 0;
}
