# LR-AEAD microcontroller firmware

This firmware implements the LR-AEAD encryption scheme which is evaluated in "Retrofitting Leakage Resilient Authenticated Encryption to Microcontrollers".

## Supported devices

The repository contains code for the [EFM32 Pearl Gecko P12 Starter Kit](https://www.silabs.com/products/development-tools/mcu/32-bit/efm32-pearl-gecko-pg12-starter-kit)
and the [STM32F2 target board](https://wiki.newae.com/CW308T-STM32F) for the [ChipWhisperer CW308 mainboard](https://wiki.newae.com/CW308_UFO_Target).


# Build

In order to build the firmware image for the EFM32PG12 microcontroller, use the
following command:

```bash
 $ make TARGET=efm32
```

The resulting firmware image **firmware.elf** can be found in **build/efm32**.
Likewise, the firmware image for the STM32F2 target can be built by using
```TARGET=stm32``` instead.


# Usage

After startup, the firmware decrypts a ciphertext using the LR-AEAD
implementation and, when successful, turns on an LED.

The *LED 1* and *LED 3* is used on the EFM32PG12 starter kit and STM32F2 target
board, respectively.

The testdata is contained in **lraead_testdata.h**. To generate your own
testdata, use the host tools to generate a header file from binary input files
and replace the contents.
