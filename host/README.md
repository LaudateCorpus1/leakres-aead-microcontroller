# LR-AEAD host tool

This tool implements the LR-AEAD encryption scheme which is evaluated in "Retrofitting Leakage Resilient Authenticated Encryption to Microcontrollers".

The inputs are provided as binary files and the output is printed to stdout in
the format of a C include file.
The purpose of this tool is to generate an encrypted image which can be included
in the microcontroller code for evaluation on the target platforms.


# Build

Building links against the mbedtls library which is included as submodule.
To build the library run (required once):

```bash
make mbedtls
```

then, to (re-)build the tool
```bash
make
```

# Usage

```bash
./lraead_gen_testvector adata_filename message_filename key_filename nonce_filename [data_complexity]
```

| Parameter        | Description                                                                      |
|------------------| ---------------------------------------------------------------------------------|
| adata_filename   | associated data (AD), can be empty or arbitrary length                           |
| message_filename | message that is encrypted, arbitrary length                                      |
| key_filename     | contains encryption and MAC key, both 16 bytes                                   |
| nonce_filename   | 16 byte nonce                                                                    |
| data_complexity  | optional, sets data complexity for the LR-PRF. Can be 2, 4, 16, 256. Default: 2. |


# Example

Example files are located under /example.

To generate an include file with AD:
```bash
./lraead_gen_testvector examples/adata.bin examples/msg.bin examples/keys.bin examples/nonce.bin
```

To generate an include file without AD:
```bash
./lraead_gen_testvector examples/empty.bin examples/msg.bin examples/keys.bin examples/nonce.bin
```
