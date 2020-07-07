# Leakage Resilient Authenticated Encryption 

This repository contains LR-AEAD implementations that are used in the paper "Retrofitting Leakage Resilient Authenticated Encryption to Microcontrollers".
It uses the LR-PRF described in [Towards Super-Exponential Side-Channel Security with Efficient Leakage-Resilient PRFs](https://link.springer.com/chapter/10.1007%2F978-3-642-33027-8_12)
in the FHGF' construction described in [Sponges Resist Leakage: The Case of Authenticated Encryption](https://link.springer.com/chapter/10.1007%2F978-3-030-34621-8_8).


# Repository

The repository is made up of two two software projects that can be used as basis
for a side-channel protected firmware upgrade process for embedded devices.

| Folder                 | Description                                                      |
| -----------------------| -----------------------------------------------------------------|
| [host](./host)         | Host software to encrypt data with the LR-AEAD scheme            |
| [firmware](./firmware) | Microcontroller firmware to decrypt data with the LR-AEAD scheme |


# License

This repository is released under the [Apache v2 License](LICENSE.txt).
